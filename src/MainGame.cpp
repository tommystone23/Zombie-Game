#include "MainGame.h"
#include <SDL2/SDL.h>
#include "Engine/Engine.h"
#include "Engine/Timing.h"
#include "Zombie.h"
#include <random>
#include <ctime>
#include "Gun.h"
#include "Engine/ResourceManager.h"
#include <glm/gtx/rotate_vector.hpp>


const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.5f;
const float PLAYER_SPEED = 3.0f;
const int FONT_SIZE = 64;
const float FONT_SCALE = 1.0f;

MainGame::MainGame() 
    : _screen_width(1920),
    _screen_height(1080), 
    _game_state(GameState::PLAY), 
    _player(NULL), 
    _fps(0),
    _cur_level(0)
{
}

MainGame::~MainGame()
{
    delete _player;
    // Remove player from human vector
    _humans[0] = _humans.back();
    _humans.pop_back();
    for(auto level : _levels)
        delete level;
    for(auto human : _humans)
        delete human;
    for(auto zombie : _zombies)
        delete zombie;
    delete _sprite_font;
    _levels.clear();
    _humans.clear();
    _zombies.clear();
}

void MainGame::run()
{
    init_systems();
    init_shaders();
    init_level();

    _sprite_batch.init();
    _hud_sprite_batch.init();
    _particle_sprite_batch.init();

    _sprite_font = new SpriteFont("fonts/OpenSans-Regular.ttf", FONT_SIZE);

    _camera.init(_screen_width, _screen_height);
    _camera.set_scale(1.0f);

    // Init particles
    GLTexture texture = ResourceManager::get_texture("textures/blood.png");
    _blood_particle_batch = new ParticleBatch2D(1000, 0.05f, texture,
                            [](Particle &particle, float delta_time, float decay_rate) {
                                particle.position += particle.velocity * delta_time;
                                particle.life_time -= decay_rate * delta_time;
                                // Add fade to particles
                                particle.color.a = (GLubyte)(particle.life_time * 255.0f);
                            });
    _particle_engine.add_particle_batch(_blood_particle_batch);

    //glEnable(GL_DEPTH_TEST);
    game_loop();
}

void MainGame::init_systems()
{
    Engine::init();
    _window.create("Zombie Game", _screen_width, _screen_height, 0);
    _audio.init();
    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
}

void MainGame::init_level()
{
    _levels.push_back(new Level("levels/level1.txt"));
    _cur_level = 0;

    _player = new Player();
    _player->init(PLAYER_SPEED, _levels[_cur_level]->get_player_start_position(), &_input_manager, &_camera, &_bullets);

    _humans.push_back(_player);

    std::mt19937 random_engine;
    random_engine.seed(time(nullptr));
    std::uniform_int_distribution<int> rand_xpos(1, _levels[_cur_level]->get_level_width() - 2);
    std::uniform_int_distribution<int> rand_ypos(1, _levels[_cur_level]->get_level_height() - 2);


    int num_humans = _levels[_cur_level]->get_num_humans();
    glm::vec2 positions[num_humans];
    memset(positions, 0, sizeof(glm::vec2) * num_humans);

    // Add all random humans
    for(int i = 0; i < num_humans; i++)
    {
        Human *human = new Human;
        do {
            glm::vec2 position((float)rand_xpos(random_engine) * TILE_WIDTH, (float)rand_ypos(random_engine) * TILE_WIDTH);
            human->init(HUMAN_SPEED, position);
            // Make sure humans are not spawned on top of eachother
            if(std::find(positions, positions + num_humans, position) != positions + num_humans) {
                continue;
            }
            // Make sure humans are not spawned on walls
            if(!human->handle_wall_collision(_levels[_cur_level]->get_level_data()))
                break;

            positions[i] = position;
        } while(true);

        _humans.push_back(human);
    }

    // Add zombie(s)
    const std::vector<glm::vec2>& zombie_positions = _levels[_cur_level]->get_zombie_start_position();
    for(int i = 0; i < zombie_positions.size(); i++)
    {
        _zombies.push_back(new Zombie);
        _zombies.back()->init(ZOMBIE_SPEED, zombie_positions[i]);
    }

    // Setup players guns
    _player->add_gun(new Gun("Pistol", 30, 0.1f, 1, 30.0f, 20.0f));
    _player->add_gun(new Gun("Shotgun", 60, 1.0f, 40, 4.0f, 20.0f));
    _player->add_gun(new Gun("Machine-Gun", 5, 0.1f, 1, 20.0f, 20.0f));
}

void MainGame::init_shaders()
{
    _shader_program.compile_shaders("shaders/texture_shader.vert", "shaders/texture_shader.frag");
    _shader_program.link_shaders();

    _font_program.compile_shaders("shaders/font_shader.vert", "shaders/font_shader.frag");
    _font_program.link_shaders();
}

void MainGame::game_loop()
{
    const float DESIRED_FPS = 60.0f;
    const int MAX_PHYSICS_STEPS = 6;
    const float MAX_DELTA_TIME = 1.0f;

    FPSLimiter fps;
    // Set max fps
    fps.init(DESIRED_FPS);

    const float MS_PER_SECOND = 1000;
    const float DESIRED_FRAME_TIME = MS_PER_SECOND / DESIRED_FPS;

    float previous_ticks = SDL_GetTicks64();

    while(_game_state == GameState::PLAY)
    {
        float new_ticks = SDL_GetTicks64();
        float frame_time = SDL_GetTicks64() - previous_ticks;
        previous_ticks = new_ticks;

        float total_delta_time  = frame_time / DESIRED_FRAME_TIME;

        fps.begin();

        process_input();

        /*
            Go through each time step and do physics calculations based
            on steps so changes in FPS will not affect collision and other physics.
        */
        int i = 0;
        while(total_delta_time > 0.0f  && i++ < MAX_PHYSICS_STEPS) {
            float delta_time = std::min(total_delta_time, MAX_DELTA_TIME);
            update_entities(delta_time);
            update_bullets(delta_time);
            _particle_engine.update(delta_time);
            total_delta_time -= delta_time;
        }

        _camera.set_position(_player->get_position());
        _camera.update();

        draw_game();

        _fps = fps.end();
    }
}

void MainGame::update_entities(float delta_time)
{
    // Update humans
    for(int i = 0; i < _humans.size(); i++)
    {
        _humans[i]->update(_levels[_cur_level]->get_level_data(), _humans, _zombies, delta_time, _audio);
    }
    // Update zombies
    for(int i = 0; i < _zombies.size(); i++)
    {
        _zombies[i]->update(_levels[_cur_level]->get_level_data(), _humans, _zombies, delta_time, _audio);
    }

    // Update human collisions
    for(int i = 0; i < _humans.size(); i++)
    {
        for(int j = i + 1; j < _humans.size(); j++)
        {
            _humans[i]->handle_entity_collision(_humans[j]);
        }
    }

    // Update zombie collisions
    for(int i = 0; i < _zombies.size(); i++)
    {
        // Zombie to zombie collision
        for(int j = i + 1; j < _zombies.size(); j++)
        {
            _zombies[i]->handle_entity_collision(_zombies[j]);
        }
        // Zombie to human collision
        for(int j = 1; j < _humans.size(); j++)
        {
            if(_zombies[i]->handle_entity_collision(_humans[j]))
            {
                // Add new zombie
                _zombies.push_back(new Zombie);
                _zombies.back()->init(ZOMBIE_SPEED, _humans[j]->get_position());
                // Delete human
                delete _humans[j];
                _humans[j] = _humans.back();
                _humans.pop_back();
            }
        }
        // Collide with player
        if(_zombies[i]->handle_entity_collision(_player))
        {
            printf("YOU LOSE");
            _game_state = GameState::EXIT;
        }
    }
}

void MainGame::update_bullets(float delta_time)
{
    // Collide with world
    for(int i = 0; i < _bullets.size();)
    {
        // If true. collided with wall
        if(_bullets[i].update(_levels[_cur_level]->get_level_data(), delta_time))
        {
            _bullets[i] = _bullets.back();
            _bullets.pop_back();
        }
        else
        {
            i++;
        }
    }

    // Collide with humans and zomies
    for(int i = 0; i < _bullets.size(); i++)
    {
        bool has_collided = false;
        // Zombies
        for(int j = 0; j < _zombies.size();)
        {
            // Check zombie collision
            if(_bullets[i].handle_entity_collision(_zombies[j]))
            {
                add_blood(_zombies[j]->get_position(), 5);
                // Damage zombie and kill if out of health
                if(_zombies[j]->apply_damage(_bullets[i].get_damage()))
                {
                    delete _zombies[j];
                    _zombies[j] = _zombies.back();
                    _zombies.pop_back();
                }
                // Remove bullet
                _bullets[i] = _bullets.back();
                _bullets.pop_back();
                has_collided = true;
                break;
            }
            j++;
        }
        // Humans
        if(has_collided)
            continue;
        // Start loop at 1 so does not collide with player
        for(int j = 1; j < _humans.size();)
        {
            // Check human collision
            if(_bullets[i].handle_entity_collision(_humans[j]))
            {
                add_blood(_humans[j]->get_position(), 5);
                // Damage human and kill if out of health
                if(_humans[j]->apply_damage(_bullets[i].get_damage()))
                {
                    delete _humans[j];
                    _humans[j] = _humans.back();
                    _humans.pop_back();
                }
                else
                {
                    j++;
                }
                // Remove bullet
                _bullets[i] = _bullets.back();
                _bullets.pop_back();
                break;
            }
            j++;
        }
    }
}

void MainGame::process_input()
{
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case(SDL_QUIT):
                _game_state = GameState::EXIT;
                break;
            case(SDL_MOUSEMOTION):
                _input_manager.set_mouse_coords(event.motion.x, event.motion.y);
                break;
            case(SDL_KEYDOWN):
                _input_manager.press_key(event.key.keysym.sym);
                break;
            case(SDL_KEYUP):
                _input_manager.release_key(event.key.keysym.sym);
                break;
            case(SDL_MOUSEBUTTONDOWN):
                _input_manager.press_key(event.button.button);
                break;
            case(SDL_MOUSEBUTTONUP):
                _input_manager.release_key(event.button.button);
                break;
        }
    }
}

void MainGame::draw_game()
{
    // Set base depth to 1
    glClearDepth(1.0);
    // Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _shader_program.use();

    glActiveTexture(GL_TEXTURE0);
    GLint texture_uniform = _shader_program.get_uniform_location("texture0");
    // Pass uniform 0 for texture 0
    glUniform1i(texture_uniform, 0);

    glm::mat4 projection = _camera.get_camera_matrix();
    GLint projection_location = _shader_program.get_uniform_location("projection");
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, &projection[0][0]);

    // Draw the level
    _levels[_cur_level]->draw();

    draw_hud();

    _sprite_batch.begin();

    _particle_engine.draw(_particle_sprite_batch);

    const glm::vec2 entity_dimensions = glm::vec2(ENTITY_RADIUS * 2);
    // Draw Humans
    for(int i = 0; i < _humans.size(); i++) {
        if(_camera.is_in_view(_humans[i]->get_position(), entity_dimensions))
            _humans[i]->draw(_sprite_batch);
    }
    // Draw zombies
    for(int i = 0; i < _zombies.size(); i++) {
        if(_camera.is_in_view(_zombies[i]->get_position(), entity_dimensions))
            _zombies[i]->draw(_sprite_batch);
    }
    // Draw bullets
    for(int i = 0; i < _bullets.size(); i++)
        _bullets[i].draw(_sprite_batch);

    _sprite_batch.end();
    _sprite_batch.render_batch();

    _shader_program.unuse();
    _window.swap_buffers();
}

void MainGame::check_victory()
{
    if(_zombies.empty())
    {
        printf("YOU WON");
        _game_state = GameState::EXIT;
    }
}

void MainGame::draw_hud()
{
    static char humans_buffer[256];
    static char zombies_buffer[256];
    static char fps_buffer[16];
    _hud_sprite_batch.begin();

    snprintf(humans_buffer, sizeof(humans_buffer), "Humans: %ld", _humans.size());
    snprintf(zombies_buffer, sizeof(zombies_buffer), "Zombies: %ld", _zombies.size());
    snprintf(fps_buffer, sizeof(fps_buffer), "FPS: %d", _fps);

    // Draw relative to camera
    glm::vec2 camera_pos = _camera.get_position();
    float camera_scale = _camera.get_scale();
    
    // need to shift camera over half of screen width and height and account for scale
    float offset_height = _screen_height / 2;
    float offset_width = _screen_width / 2;

    glm::vec2 human_text_pos(camera_pos.x - (offset_width / camera_scale), 
                    camera_pos.y - ((offset_height / camera_scale) - FONT_SIZE));
    glm::vec2 zombie_text_pos(human_text_pos.x, human_text_pos.y + (FONT_SIZE * FONT_SCALE));
    glm::vec2 fps_text_pos(camera_pos.x - (offset_width / camera_scale), 
                    camera_pos.y + (offset_height / camera_scale) - (FONT_SIZE * FONT_SCALE));
    
    Color font_colour = { 0, 0, 0, 255 };
    _sprite_font->draw(_hud_sprite_batch, humans_buffer, human_text_pos, glm::vec2(FONT_SCALE), 0.0f, font_colour);
    _sprite_font->draw(_hud_sprite_batch, zombies_buffer, zombie_text_pos, glm::vec2(FONT_SCALE), 0.0f, font_colour);
    _sprite_font->draw(_hud_sprite_batch, fps_buffer, fps_text_pos, glm::vec2(FONT_SCALE), 0.0f, font_colour);

    _hud_sprite_batch.end();
    _hud_sprite_batch.render_batch();
}

void MainGame::add_blood(const glm::vec2 &position, int num_particles)
{
    static std::mt19937 random_engine(time(nullptr));
    static std::uniform_real_distribution<float> rand_angle(0.0f, 360.0f);

    glm::vec2 velocity(1.0f, 0.0f);
    Color color = { 255, 255, 255, 255 };
    for(int i = 0; i < num_particles; i++) {
        _blood_particle_batch->add_particle(position, glm::rotate(velocity, rand_angle(random_engine)), color, 5.0f);
    }
}
