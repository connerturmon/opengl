#version 330 core

out vec4 fragment_color;

uniform vec3 light_color;
uniform vec3 cube_color;

void main()
{
    fragment_color = vec4(light_color * cube_color, 1.0f);
}