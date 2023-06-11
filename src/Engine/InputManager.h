#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include <unordered_map>
#include <glm/glm.hpp>

class InputManager
{
public:
    InputManager();
    ~InputManager();

    void press_key(unsigned int key_id);
    void release_key(unsigned int key_id);

    bool is_key_pressed(unsigned int key_id);

    void set_mouse_coords(float x, float y);
    glm::vec2 get_mouse_coords() const { return _mouse_coords; }
private: 
    std::unordered_map<unsigned int, bool> _key_map;

    glm::vec2 _mouse_coords;
};

#endif