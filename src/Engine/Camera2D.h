#ifndef CAMERA2D
#define CAMERA2D
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera2D
{
public:
    Camera2D();
    ~Camera2D();

    void init(int screen_width, int screen_height);
    void update();

    // setters
    void set_position(const glm::vec2 &new_position) { _position = new_position; _position_changed = true; }
    void set_scale(float new_scale) { _scale = new_scale; _position_changed = true; }
    // getters
    glm::vec2 get_position() const { return _position; }
    float get_scale() const { return _scale; }
    glm::mat4 get_camera_matrix() const { return _camera_matrix; }

    glm::vec2 get_world_coords(glm::vec2 screen_coords);
private:
    bool _position_changed;
    int _screen_width;
    int _screen_height;
    float _scale;
    glm::vec2 _position;
    glm::mat4 _camera_matrix;
    // orthographic matrix
    glm::mat4 _ortho_matrix;
};


#endif