#include "Gun.h"
#include <ctime>
#include <random>
#include <glm/gtx/rotate_vector.hpp>

Gun::Gun(std::string name, int fire_rate, float spread, int bullets_per_shot,
        float bullet_damage, float bullet_speed) :
        _name(name),
        _fire_rate(fire_rate),
        _spread(spread),
        _bullets_per_shot(bullets_per_shot),
        _bullet_speed(bullet_speed),
        _bullet_damage(bullet_damage),
        _frame_counter(0)
{
}

Gun::~Gun()
{
}

void Gun::update(bool is_mouse_pressed, const glm::vec2& position, 
                const glm::vec2& direction, std::vector<Bullet> &bullets,
                float delta_time)
{
    _frame_counter += 1.0f * delta_time;
    if(_frame_counter >= _fire_rate && is_mouse_pressed) 
    {
        fire(direction, position, bullets);
        _frame_counter = 0;
    }
}

void Gun::fire(const glm::vec2& direction, const glm::vec2& position, std::vector<Bullet> &bullets)
{
    static std::mt19937 random_engine(time(nullptr));
    std::uniform_real_distribution<float> random_direction(-_spread, _spread);

    for(int i = 0; i < _bullets_per_shot; i++)
    {
        bullets.emplace_back(position, glm::rotate(direction, random_direction(random_engine)), _bullet_damage, _bullet_speed);
    }
}
