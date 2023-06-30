#include "Level.h"
#include <fstream>
#include <iostream>
#include "Engine/Errors.h"
#include "Engine/ResourceManager.h"

Level::Level(const std::string &file_name)
{
    std::ifstream file;
    file.open(file_name);

    // Error checking
    if(file.fail())
        fatal_error("Failed to open " + file_name);

    std::string temp;
    // Read first string to temp and second string into _num_humans
    file >> temp >> _num_humans;

    // Throw away first line so it does not interfere with level data
    std::getline(file, temp);
    while(std::getline(file, temp))
    {
        _level_data.push_back(temp);
    }

    _sprite_batch.init();
    _sprite_batch.begin();

    // Same texture coords for all tectures
    glm::vec4 uv_rect(0.0f, 0.0f, 1.0f, 1.0f);
    //glm::vec4 uv_rect(0.0625f, 0.0625f, 0.9375f, 0.9375f);
    Color color = { 255, 255, 255, 255};

    for(int y = 0; y < _level_data.size();y++)
    {
        for(int x = 0; x < _level_data[y].size(); x++)
        {
            char tile = _level_data[y][x];

            // Get destination rectangle
            glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

            switch(tile)
            {
                case('B'):
                    _sprite_batch.draw(destRect, uv_rect, ResourceManager::get_texture("textures/brick_texture.png").id, 0.0f, color);
                    break;
                case('R'):
                    _sprite_batch.draw(destRect, uv_rect, ResourceManager::get_texture("textures/brick_texture.png").id, 0.0f, color);
                    break;
                case('@'):
                    _level_data[y][x] = '.';
                    _player_start_position.x = x * TILE_WIDTH;
                    _player_start_position.y = y * TILE_WIDTH;
                    _sprite_batch.draw(destRect, uv_rect, ResourceManager::get_texture("textures/stone_floor.png").id, 0.0f, color);
                    break;
                case('Z'):
                    _level_data[y][x] = '.';
                    _zombie_start_positions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
                    _sprite_batch.draw(destRect, uv_rect, ResourceManager::get_texture("textures/stone_floor.png").id, 0.0f, color);
                    break;
                case('.'):
                    _sprite_batch.draw(destRect, uv_rect, ResourceManager::get_texture("textures/stone_floor.png").id, 0.0f, color);
                    break;
                default:
                    std::cout << "Unexpected symbol in level file" << std::endl;
                    break;
            }
        }
    }
    _sprite_batch.end();
}

Level::~Level()
{

}

void Level::draw()
{
    _sprite_batch.render_batch();
}