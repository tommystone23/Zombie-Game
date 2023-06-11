#include "Bullet.h"
#include "Engine/ResourceManager.h"
#include "Level.h"

Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed) :
            _position(position),
            _direction(direction),
            _damage(damage),
            _speed(speed)
{
}

Bullet::~Bullet()
{
}

bool Bullet::update(const std::vector<std::string> &level_data)
{
    _position += _direction * _speed;
    return handle_wall_collision(level_data);
}

void Bullet::draw(SpriteBatch &sprite_batch)
{
    const GLuint texture_id = ResourceManager::get_texture("textures/character.png").id;
    glm::vec4 dest_rect(_position.x + BULLET_RADIUS, _position.y + BULLET_RADIUS,
                        BULLET_RADIUS * 2, BULLET_RADIUS * 2);
    const glm::vec4 uv_rect(0.0f, 0.0f, 1.0f, 1.0f);
    Color color;
    color.r = 0;
    color.g = 0;
    color.b = 0;
    color.a = 255;

    sprite_batch.draw(dest_rect, uv_rect, texture_id, 0.0f, color);
}

bool Bullet::handle_entity_collision(Entity *entity)
{
const float MIN_DISTANCE = ENTITY_RADIUS + BULLET_RADIUS;

    glm::vec2 centre_pos1 = _position;
    glm::vec2 centre_pos2 = entity->get_position() + glm::vec2(ENTITY_RADIUS);

    glm::vec2 distance_vector = centre_pos1 - centre_pos2;

    float distance = glm::length(distance_vector);
    float collision_depth = MIN_DISTANCE - distance;

    // Check for collision
    if(collision_depth > 0)
        return true;

    return false;
}

bool Bullet::handle_wall_collision(const std::vector<std::string> &level_data)
{
    glm::vec2 corner_pos = glm::vec2(floor(_position.x / (float)TILE_WIDTH), 
                                        floor(_position.y / (float)TILE_WIDTH));
    
    // Check if entity is outside map or inside wall
    if(corner_pos.x < 0 || corner_pos.x >= level_data[0].size() ||
        corner_pos.y < 0 || corner_pos.y >= level_data.size())
        {
            return true;
        }

    // Check if collides
    return level_data[corner_pos.y][corner_pos.x] != '.';
}
