#version 330 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec4 vertex_color;
layout (location = 2) in vec2 vertex_uv;

out vec4 fragment_color;
out vec2 fragment_uv;

uniform mat4 projection;

void main()
{
    // Convert vertex position to vec4 so it can be multiplied by mat4
    gl_Position.xyz = (projection * vec4(vertex_position, 1.0)).xyz;
    gl_Position.w = 1.0f;

    // Set out variable to be input vertex_color
    fragment_color = vertex_color;
    fragment_uv = vec2(vertex_uv.x, 1.0 - vertex_uv.y);
}