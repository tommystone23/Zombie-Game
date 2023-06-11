#ifndef SPRITE_H
#define SPRITE_H
#include "GL/glew.h"
#include "Vertex.h"
#include "GLTexture.h"
#include <string>

class Sprite
{
public:
    Sprite();
    ~Sprite();

    void init(float x, float y, float width, float height, std::string texture_file_path);
    void draw();

private:
    // Coordinates of Sprite
    int _x;
    int _y;
    int _width;
    int _height;
    // Vertex Buffer ID
    GLuint _VBO;
    // Texture struct
    GLTexture _texture;

};


#endif