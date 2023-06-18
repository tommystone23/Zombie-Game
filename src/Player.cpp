#include "Player.h"
#include "SDL2/SDL.h"


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

    _color = { 0, 0, 128, 255 };
}

void Player::update(const std::vector<std::string> &level_data,
                        std::vector<Human*> &humans, std::vector<Zombie*> &zombies,
                        float delta_time)
{
    if(_input_manager->is_key_pressed(SDLK_w))
        _position.y += _speed * delta_time;
    if(_input_manager->is_key_pressed(SDLK_s))
        _position.y -= _speed * delta_time;
    if(_input_manager->is_key_pressed(SDLK_a))
        _position.x -= _speed * delta_time;
    if(_input_manager->is_key_pressed(SDLK_d))
        _position.x += _speed * delta_time;

    if(_input_manager->is_key_pressed(SDLK_1) && _guns.size() >= 0)
        _cur_gun_index = 0;
    if(_input_manager->is_key_pressed(SDLK_2) && _guns.size() >= 1)
        _cur_gun_index = 1;
    if(_input_manager->is_key_pressed(SDLK_3) && _guns.size() >= 2)
        _cur_gun_index = 2;

    if(_cur_gun_index != -1)
    {
        glm::vec2 mouse_coords = _input_manager->get_mouse_coords();
        mouse_coords = _camera->get_world_coords(mouse_coords);
        glm::vec2 player_center_position = _position + glm::vec2(ENTITY_RADIUS);
        glm::vec2 direction = glm::normalize(mouse_coords - player_center_position);
        _guns[_cur_gun_index]->update(_input_manager->is_key_pressed(SDL_BUTTON_LEFT),
                                        player_center_position, 
                                        direction,
                                        *_bullets, delta_time);
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
