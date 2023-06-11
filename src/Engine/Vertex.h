#ifndef VERTEX_H
#define VERTEX_H
#include "GL/glew.h"

struct Position
{
    float x;
    float y;
    float z;
};

struct Color
{
    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
};

struct UV
{
    float u;
    float v;
};

struct Vertex
{   
    Position position;
    Color color;
    UV uv;

    void set_position(float x, float y, float z)
    {
        position.x = x;
        position.y = y;
        position.z = z;
    }

    void set_color(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
    {
        color.r = r;
        color.g = g;
        color.b = b;
        color.a = a;
    }

    void set_uv(float u, float v)
    {
        uv.u = u;
        uv.v = v;
    }
};

#endif