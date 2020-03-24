#version 330 core

out vec4 frag_color;
in vec2 texture_coords;

uniform sampler2D our_texture;

void main()
{
    frag_color = texture(our_texture, texture_coords);
}