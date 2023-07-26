#include "Entity.h"
#include "Engine/ResourceManager.h"
#include "Level.h"
#include <algorithm>

Entity::Entity()
{
    _direction = glm::vec2(0.0f, 1.0f);
}

Entity::~Entity()
{
}

void Entity::draw(SpriteBatch &sprite_batch)
{
    // Texture coords will always be the same.
    const glm::vec4 uv_rect(0.0f, 0.0f, 1.0f, 1.0f);

    glm::vec4 dest_rect(_position.x, _position.y, ENTITY_WIDTH, ENTITY_WIDTH);
    sprite_batch.draw(dest_rect, uv_rect, _texture_id, 0, _color, _direction);
}

bool Entity::handle_wall_collision(const std::vector<std::string> &level_data)
{
    std::vector<glm::vec2> collide_wall_position;

    // Check 4 corners
    // First corner
    check_wall_position(level_data, collide_wall_position, _position.x, _position.y);
    //Second corner
    check_wall_position(level_data, collide_wall_position, _position.x + ENTITY_WIDTH, _position.y);
    // Third corner
    check_wall_position(level_data, collide_wall_position, _position.x, _position.y + ENTITY_WIDTH);
    // Fourth corner
    check_wall_position(level_data, collide_wall_position, _position.x + ENTITY_WIDTH, _position.y + ENTITY_WIDTH);

    if(collide_wall_position.size() == 0)
        return false;

    for(size_t i = 0; i < collide_wall_position.size(); i++){
        collide_with_wall(collide_wall_position[i]);
    }
    return true;
}

// Uses circular collision
bool Entity::handle_entity_collision(Entity *entity)
{
    // Minimum distance between entities to result in collision
    const float MIN_DISTANCE = ENTITY_RADIUS * 2;

    glm::vec2 centre_pos1 = _position + glm::vec2(ENTITY_RADIUS);
    glm::vec2 centre_pos2 = entity->get_position() + glm::vec2(ENTITY_RADIUS);

    glm::vec2 distance_vector = centre_pos1 - centre_pos2;

    float distance = glm::length(distance_vector);
    float collision_depth = MIN_DISTANCE - distance;

    // Check for collision
    if(collision_depth > 0)
    {
        glm::vec2 collison_depth_vector = glm::normalize(distance_vector) * collision_depth;

        _position += collison_depth_vector / 2.0f;
        entity->_position -= collison_depth_vector / 2.0f;
        return true;
    }

    return false;
}

bool Entity::apply_damage(float damage)
{
    _health -= damage;
    if(_health <= 0)
        return true;
    return false;
}

void Entity::check_wall_position(const std::vector<std::string> &level_data, 
            std::vector<glm::vec2> &collide_wall_position, float x, float y)
{
    glm::vec2 corner_pos(std::floor(x / TILE_WIDTH), 
                            std::floor(y / TILE_WIDTH));
    
    // Check if entity is outside map or inside wall
    if(corner_pos.x < 0 || corner_pos.x >= level_data[0].size() ||
        corner_pos.y < 0 || corner_pos.y >= level_data.size())
        {
            return;
        }

    // Weird bug where position becomes NaN
    if(isnan(_position.x) || isnan(_position.y))
        return;


    // Check if collideable
    if(level_data[corner_pos.y][corner_pos.x] != '.'){
        collide_wall_position.push_back(corner_pos  * TILE_WIDTH + glm::vec2(TILE_WIDTH / 2.0f));
    }
}

// AABB collision
void Entity::collide_with_wall(glm::vec2 tile_pos)
{
    const float ENTITY_RADIUS = ENTITY_WIDTH / 2.0f;
    const float WALL_RADIUS = TILE_WIDTH / 2.0f;
    const float MIN_DISTANCE = ENTITY_RADIUS + WALL_RADIUS;

    glm::vec2 center_player = _position + glm::vec2(ENTITY_RADIUS);
    glm::vec2 distance_vector = center_player - tile_pos;

    float x_depth = MIN_DISTANCE - abs(distance_vector.x);
    float y_depth = MIN_DISTANCE - abs(distance_vector.y);

    // If this is true we are colliding
    if(x_depth > 0 || y_depth > 0)
    {
        if(std::max(x_depth, 0.0f) < std::max(y_depth, 0.0f))
        {
            if(distance_vector.x < 0)
                _position.x -= x_depth;
            else
                _position.x += x_depth;
        }
        else
        {
            if(distance_vector.y < 0)
                _position.y -= y_depth;
            else
                _position.y += y_depth;
        }
    }
}
