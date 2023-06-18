#ifndef BULLET_H
#define BULLET_H
#include <glm/glm.hpp>
#include "Engine/SpriteBatch.h"
#include "Entity.h"

class Human;
class Zombie;

const int BULLET_RADIUS = 5;

class Bullet
{
public:
    Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed);
    ~Bullet();

    bool update(const std::vector<std::string> &level_data, float delta_time);
    void draw(SpriteBatch &sprite_batch);

    bool handle_entity_collision(Entity *entity);
    float get_damage() const { return _damage; }
private:
    glm::vec2 _position;
    glm::vec2 _direction;
    float _damage;
    float _speed;

    bool handle_wall_collision(const std::vector<std::string> &level_data);
};
#endif