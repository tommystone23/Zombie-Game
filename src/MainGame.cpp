#include "MainGame.h"
#include <SDL2/SDL.h>
#include "Engine/Engine.h"
#include "Engine/Timing.h"
#include "Zombie.h"
#include <random>
#include <ctime>
#include "Gun.h"


const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.5f;
const float PLAYER_SPEED = 3.0f;

MainGame::MainGame() : _screen_width(1920), _screen_height(1080), _game_state(GameState::PLAY), _fps(0), _player(NULL)
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
    _camera.init(_screen_width, _screen_height);
    glEnable(GL_DEPTH_TEST);
    game_loop();
}

void MainGame::init_systems()
{
    Engine::init();
    _window.create("Zombie Game", _screen_width, _screen_height, 0);
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

    // Add all random humans
    for(int i = 0; i < _levels[_cur_level]->get_num_humans(); i++)
    {
        _humans.push_back(new Human);
        glm::vec2 position(rand_xpos(random_engine) * TILE_WIDTH, rand_ypos(random_engine) * TILE_WIDTH);
        _humans.back()->init(HUMAN_SPEED, position);
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
        _humans[i]->update(_levels[_cur_level]->get_level_data(), _humans, _zombies, delta_time);
    }
    // Update zombies
    for(int i = 0; i < _zombies.size(); i++)
    {
        _zombies[i]->update(_levels[_cur_level]->get_level_data(), _humans, _zombies, delta_time);
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
            // Check zombie collision
            if(_bullets[i].handle_entity_collision(_humans[j]))
            {
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

    _sprite_batch.begin();

    // Draw Humans
    for(int i = 0; i < _humans.size(); i++)
        _humans[i]->draw(_sprite_batch);
    // Draw zombies
    for(int i = 0; i < _zombies.size(); i++)
        _zombies[i]->draw(_sprite_batch);
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
