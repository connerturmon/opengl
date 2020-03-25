#version 330 core
layout (location = 0) in vec3 apos;
layout (location = 1) in vec3 anormal;

out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 fragment_position;

void main()
{
    gl_Position = projection * view * model * vec4(apos, 1.0f);
    fragment_position = vec3(model * vec4(apos, 1.0f));
    normal = anormal;
}