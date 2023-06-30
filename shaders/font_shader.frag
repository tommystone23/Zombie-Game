#version 330 core

in vec4 fragment_color;
in vec2 fragment_uv;

out vec4 final_color;

uniform sampler2D texture0;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(texture0, fragment_uv).r);
    final_color = vec4(fragment_color) * sampled;
}