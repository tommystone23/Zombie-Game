#ifndef MAINGAME_H
#define MAINGAME_H

#include "Engine/InputManager.h"
#include "Engine/Window.h"
#include "Engine/GLSLProgram.h"
#include "Engine/Camera2D.h"
#include "Engine/SpriteBatch.h"
#include "Level.h"
#include <vector>
#include "Player.h" 
#include "Bullet.h"
#include "Engine/SpriteFont.h"
#include "Engine/ParticleEngine2D.h"
#include "Engine/ParticleBatch2D.h"

class Zombie;

enum class GameState{
    PLAY,
    EXIT
};

class MainGame
{
public:
    MainGame();
    ~MainGame();

    void run();
private:
    // Initialize startup libraries/systems
    void init_systems();
    // Initialize Level
    void init_level();
    // Compile and link shaders
    void init_shaders();
    // Main game loop
    void game_loop();
    // Update all entities
    void update_entities(float delta_time);
    // Update all bullets
    void update_bullets(float delta_time);
    // Get user input
    void process_input();
    // Draw graphics to the screen
    void draw_game();
    // Draws the HUD
    void draw_hud();
    // Check if player won
    void check_victory();
    // Adds blood to particle engine
    void add_blood(const glm::vec2 &position, int num_particles);

    int _screen_width;
    int _screen_height;

    Window _window;

    GLSLProgram _shader_program;
    GLSLProgram _font_program;

    InputManager _input_manager;

    Camera2D _camera;

    GameState _game_state;

    SpriteBatch _sprite_batch;
    SpriteBatch _hud_sprite_batch;
    SpriteBatch _particle_sprite_batch;
    SpriteFont *_sprite_font;

    ParticleEngine2D _particle_engine;
    ParticleBatch2D *_blood_particle_batch;

    AudioEngine _audio;

    Player *_player;
    std::vector<Level*> _levels;
    std::vector<Human*> _humans;
    std::vector<Zombie*> _zombies;
    std::vector<Bullet> _bullets;

    int _fps;
    int _cur_level;
};

#endif