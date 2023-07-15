#ifndef GUN_H
#define GUN_H
#include <string>
#include "Bullet.h"

class Gun
{
public:
    Gun(std::string name, int fire_rate, float spread, int bullets_per_shot,
        float bullet_damage, float bullet_speed);
    ~Gun();

    void update(bool is_mouse_pressed, const glm::vec2& position, 
                const glm::vec2& direction, std::vector<Bullet> &bullets,
                float delta_time, AudioEngine &audio);
private:
    std::string _name;
    int _fire_rate; // Fire rate in terms of frames
    float _spread; // Accuracy
    int _bullets_per_shot;
    float _bullet_speed;
    float _bullet_damage;

    float _frame_counter;

    void fire(const glm::vec2& direction, const glm::vec2& position, std::vector<Bullet> &bullets);
    void emit_gun_sound(AudioEngine &audio);
};

#endif