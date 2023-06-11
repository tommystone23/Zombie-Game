#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include "Engine/SpriteBatch.h"
#include "glm/glm.hpp"

// Pixel width of each tile
const int TILE_WIDTH = 64;

class Level
{
public:
    // Load the level
    Level(const std::string &file_name);
    ~Level();

    glm::vec2 get_player_start_position() const { return _player_start_position; }
    const std::vector<glm::vec2>& get_zombie_start_position() const { return _zombie_start_positions; }
    const std::vector<std::string>& get_level_data() const { return _level_data; }
    int get_num_humans() const { return _num_humans; }
    int get_level_width() const { return _level_data[0].size(); }
    int get_level_height() const { return _level_data.size(); }


    void draw();
private:
    std::vector<std::string> _level_data;
    int _num_humans;

    SpriteBatch _sprite_batch;

    glm::vec2 _player_start_position;
    std::vector<glm::vec2> _zombie_start_positions;
};

#endif