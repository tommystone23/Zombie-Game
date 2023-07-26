#include "Sprite.h"
#include <cstddef>
#include "ResourceManager.h"

Sprite::Sprite()
{
    _VBO = 0;
}

Sprite::~Sprite()
{
    if(_VBO != 0)
    {
        glDeleteBuffers(1, &_VBO);
    }
}

void Sprite::init(float x, float y, float width, float height, std::string texture_file_path)
{
    _x = x;
    _y = y;
    _width = width;
    _height = height;

    _texture = ResourceManager::get_texture(texture_file_path);

    // Generate buffers
    if(_VBO == 0)
    {
        glGenBuffers(1, &_VBO);
    }

    // Use Vertex struct for each vertex
    Vertex vertices[6];
    // First triangle
    vertices[0].set_position(x, y);
    vertices[0].set_color(0, 255, 255, 255);
    vertices[0].set_uv(0.0f, 0.0f);

    vertices[1].set_position(x, (y+height));
    vertices[1].set_color(0, 255, 255, 255);
    vertices[1].set_uv(0.0f, 1.0f);

    vertices[2].set_position((x+width), (y+height));
    vertices[2].set_color(0, 255, 255, 255);
    vertices[2].set_uv(1.0f, 1.0f);

    // Second Triangle
    vertices[3].set_position(x, y);
    vertices[3].set_color(0, 255, 255, 255);
    vertices[3].set_uv(0.0f, 0.0f);

    vertices[4].set_position((x+width), y);
    vertices[4].set_color(0, 255, 255, 255);
    vertices[4].set_uv(1.0f, 0.0f);

    vertices[5].set_position((x+width), (y+height));
    vertices[5].set_color(0, 255, 255, 255);
    vertices[5].set_uv(1.0f, 1.0f);


    // Tell OpenGL to make this buffer active
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    // Upload buffer data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Unbind data
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::draw()
{
    glBindTexture(GL_TEXTURE_2D, _texture.id);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);

    // Pointer to each vertex in buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    // Pointer to color in vertex buffer. GL_TRUE because values need to be converted to floats
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);
    // Pointer to texture in vertex buffer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(2);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}