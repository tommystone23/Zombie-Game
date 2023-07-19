#include "Human.h"
#include <ctime>
#include <random>
#include <glm/gtx/rotate_vector.hpp>
#include "Engine/ResourceManager.h"

Human::Human()
{
}

Human::~Human()
{
}

void Human::init(float speed, glm::vec2 position)
{
    static std::mt19937 random_engine(time(nullptr));
    static std::uniform_real_distribution<float> random_direction(-1.0f, 1.0f);

    _color = { 255, 255, 255, 255 };

    _health = 20;
    _speed = speed;
    _position = position;
    _direction = glm::vec2(random_direction(random_engine), random_direction(random_engine));
    if(_direction.length() == 0 || isnan(_direction.x) || isnan(_direction.y))
        _direction = glm::vec2(1.0f, 0.0f);
    // Change direction vector to normalized coordinates
    _direction = glm::normalize(_direction);
    _texture_id = ResourceManager::get_texture("textures/human.png").id;
}

void Human::update(const std::vector<std::string> &level_data,
                        std::vector<Human*> &humans, std::vector<Zombie*> &zombies,
                        float delta_time, AudioEngine &audio)
{
    static std::mt19937 random_engine(time(nullptr));
    static std::uniform_real_distribution<float> random_rotate(-1.0f, 1.0f);

    _position += (_direction * _speed) * delta_time;
    // Manually set position if position become NaN
    if(isnan(_position.x) || isnan(_position.y)) {
        _position = glm::vec2(50) * ENTITY_WIDTH;
        _position += (_direction * _speed) * delta_time;
    }

    // Rotate entity if collides with wall
    if(handle_wall_collision(level_data))
        _direction = glm::rotate(_direction, random_rotate(random_engine));
}
