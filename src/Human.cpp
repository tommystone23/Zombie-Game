#include "Human.h"
#include <ctime>
#include <random>
#include <glm/gtx/rotate_vector.hpp>

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

    _color = { 200, 0, 200, 255 };

    _health = 20;
    _speed = speed;
    _position = position;
    _direction = glm::vec2(random_direction(random_engine), random_direction(random_engine));
    if(_direction.length() == 0)
        _direction = glm::vec2(1.0f, 0.0f);
    // Change direction vector to normalized coordinates
    _direction = glm::normalize(_direction);
}

void Human::update(const std::vector<std::string> &level_data,
                        std::vector<Human*> &humans, std::vector<Zombie*> &zombies,
                        float delta_time)
{
    static std::mt19937 random_engine(time(nullptr));
    static std::uniform_real_distribution<float> random_rotate(-1.0f, 1.0f);

    _position += _direction * _speed;

    // Rotate entity if collides with wall
    if(handle_wall_collision(level_data))
        _direction = glm::rotate(_direction, random_rotate(random_engine));
}
