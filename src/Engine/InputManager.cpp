#include "InputManager.h"

InputManager::InputManager() : _mouse_coords(0.0f)
{
}

InputManager::~InputManager()
{
}

void InputManager::press_key(unsigned int key_id)
{
    _key_map[key_id] = true;
}

void InputManager::release_key(unsigned int key_id)
{
    _key_map[key_id] = false;
}

bool InputManager::is_key_pressed(unsigned int key_id)
{
    if(_key_map.find(key_id) != _key_map.end())
    {
        return _key_map[key_id];
    }
    return false;
}

void InputManager::set_mouse_coords(float x, float y)
{
    _mouse_coords.x = x;
    _mouse_coords.y = y;
}
