#version 330 core
layout (location = 0) in vec3 apos;
layout (location = 1) in vec2 atexture_coords;

out vec2 texture_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(apos.x, apos.y, apos.z, 1.0f);
    texture_coords = atexture_coords;
}