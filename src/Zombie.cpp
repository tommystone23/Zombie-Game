#include "Zombie.h"
#include <glm/gtx/rotate_vector.hpp>
#include <random>
#include <ctime>

Zombie::Zombie()
{
}

Zombie::~Zombie()
{
}

void Zombie::init(float speed, glm::vec2 position)
{
    _color = { 0, 100, 160, 255 };

    _health = 150;
    _speed = speed;
    _position = position;
}

void Zombie::update(const std::vector<std::string> &level_data,
                        std::vector<Human*> &humans, std::vector<Zombie*> &zombies,
                        float delta_time)
{
    Human* nearest_human = get_nearest_human(humans);
    if(nearest_human != nullptr)
    {
        glm::vec2 direction = glm::normalize(nearest_human->get_position() - _position);
        _position += direction * _speed * delta_time;
    }

    // Rotate entity if collides with wall
    handle_wall_collision(level_data);
}

Human* Zombie::get_nearest_human(std::vector<Human*> &humans)
{
    Human* nearest_human = nullptr;
    // Arbitrarily large value to start
    float smallest_distance = 9999999.0f;

    for(int i = 0; i < humans.size(); i++)
    {
        glm::vec2 distance_vector = humans[i]->get_position() - _position;
        float distance = glm::length(distance_vector);

        if(distance < smallest_distance)
        {
            smallest_distance = distance;
            nearest_human = humans[i];
        }
    }

    return nearest_human;
}
