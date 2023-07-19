#include "Player.h"
#include "SDL2/SDL.h"
#include "Engine/ResourceManager.h"


Player::Player() : _cur_gun_index(-1)
{
}

Player::~Player()
{
    for(int i = 0; i < _guns.size(); i++)
        delete _guns[i];
}

void Player::init(int speed, glm::vec2 position, InputManager *input_manager, Camera2D *camera, std::vector<Bullet> *bullets)
{
    _speed = speed;
    _position = position;
    _input_manager = input_manager;
    _camera = camera;
    _bullets = bullets;
    _health = 150;
    _texture_id = ResourceManager::get_texture("textures/player.png").id;
    // Default direction is looking down
    _direction = glm::vec2(0.0f, -1.0f);

    _color = { 255, 255, 255, 255 };
}

void Player::update(const std::vector<std::string> &level_data,
                        std::vector<Human*> &humans, std::vector<Zombie*> &zombies,
                        float delta_time, AudioEngine &audio)
{
    glm::vec2 movement_direction(0.0f);
    if(_input_manager->is_key_pressed(SDLK_w))
        movement_direction.y += 1;
    if(_input_manager->is_key_pressed(SDLK_s))
        movement_direction.y -= 1;
    if(_input_manager->is_key_pressed(SDLK_a))
        movement_direction.x -= 1;
    if(_input_manager->is_key_pressed(SDLK_d))
        movement_direction.x += 1;

    if(glm::length(movement_direction))
        _position += glm::normalize(movement_direction) * _speed * delta_time;

    if(_input_manager->is_key_pressed(SDLK_1) && _guns.size() >= 0)
        _cur_gun_index = 0;
    if(_input_manager->is_key_pressed(SDLK_2) && _guns.size() >= 1)
        _cur_gun_index = 1;
    if(_input_manager->is_key_pressed(SDLK_3) && _guns.size() >= 2)
        _cur_gun_index = 2;

    glm::vec2 mouse_coords = _input_manager->get_mouse_coords();
    mouse_coords = _camera->get_world_coords(mouse_coords);
    glm::vec2 player_center_position = _position + glm::vec2(ENTITY_RADIUS);
    _direction = glm::normalize(mouse_coords - player_center_position);

    if(_cur_gun_index != -1)
    {
        _guns[_cur_gun_index]->update(_input_manager->is_key_pressed(SDL_BUTTON_LEFT),
                                        player_center_position, 
                                        _direction,
                                        *_bullets, delta_time, audio);
    }
    

    handle_wall_collision(level_data);
}

void Player::add_gun(Gun *gun)
{
    // Add gun to inventory
    _guns.push_back(gun);
    // If no gun equipped, equip gun
    if(_cur_gun_index == -1)
        _cur_gun_index = 0;
}
