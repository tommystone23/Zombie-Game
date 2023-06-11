#include "Camera2D.h"

Camera2D::Camera2D() 
: _position_changed(true), _scale(1), _position(0, 0), _camera_matrix(0.0f)
{

}

Camera2D::~Camera2D()
{

}

void Camera2D::init(int screen_width, int screen_height)
{
    _screen_width = screen_width;
    _screen_height = screen_height;
    // Pass in each side of the screen. Left, right, bottom, top
    _ortho_matrix = glm::ortho(0.0f, (float)_screen_width, 0.0f, (float)_screen_height);
}

void Camera2D::update()
{
    if(!_position_changed)
        return;
    
    // Translate Camera
    glm::vec3 translate(-_position.x + _screen_width/2, -_position.y + _screen_height/2, 0.0f);
    _camera_matrix = glm::translate(_ortho_matrix, translate);
    // Scale Camera
    glm::vec3 scale(_scale, _scale, 0.0f);
    _camera_matrix = glm::scale(glm::mat4(1.0f), scale) * _camera_matrix;

    _position_changed = false;
}
glm::vec2 Camera2D::get_world_coords(glm::vec2 screen_coords)
{
    // Invert Y coords
    screen_coords.y = _screen_height - screen_coords.y;
    // Make the centre 0,0
    screen_coords -= glm::vec2(_screen_width/2, _screen_height/2);
    // Scale coordinates
    screen_coords /= _scale;
    // Translate with the camera position
    screen_coords += _position;

    return screen_coords;
}


