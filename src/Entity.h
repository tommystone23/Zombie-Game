#ifndef AGENT_H
#define AGENT_H
#include "glm/glm.hpp"
#include "Engine/GLTexture.h"
#include "Engine/SpriteBatch.h"
#include <string>
#include "Engine/AudioEngine.h"

const float ENTITY_WIDTH = 60;
const float ENTITY_RADIUS = ENTITY_WIDTH / 2.0f;

// This is to avoid circular includes
class Human;
class Zombie;

class Entity
{
public:
    Entity();
    virtual ~Entity();

    void draw(SpriteBatch &sprite_batch);
    virtual void update(const std::vector<std::string> &level_data,
                        std::vector<Human*> &humans, std::vector<Zombie*> &zombies,
                        float delta_time, AudioEngine &audio) = 0;

    bool handle_wall_collision(const std::vector<std::string> &level_data);

    bool handle_entity_collision(Entity *entity);

    // Return true if we died
    bool apply_damage(float damage);

    glm::vec2 get_position() const { return _position; }

protected:
    glm::vec2 _position;
    float _speed;
    Color _color;
    float _health;

    void check_wall_position(const std::vector<std::string> &level_data, 
                std::vector<glm::vec2> &collide_tile_positon, float x, float y);
    void collide_with_wall(glm::vec2 tile_pos);
};

#endif