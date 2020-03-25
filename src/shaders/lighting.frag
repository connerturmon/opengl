#version 330 core
in vec3 normal;
in vec3 fragment_position;

out vec4 fragment_color;

uniform vec3 light_color;
uniform vec3 cube_color;
uniform vec3 light_pos;

void main()
{
    float ambient_strength = 0.1f;
    vec3 ambient_light = light_color * ambient_strength;

    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(light_pos - fragment_position);
    float diff = max(dot(norm, light_dir), 0.0f);
    vec3 diffuse = (diff * light_color);

    vec3 object_color = (ambient_light + diffuse) * cube_color;
    fragment_color = vec4(object_color, 1.0f);
}