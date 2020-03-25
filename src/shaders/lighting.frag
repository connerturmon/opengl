#version 330 core
in vec3 normal;
in vec3 fragment_position;

out vec4 fragment_color;

uniform vec3 light_color;
uniform vec3 cube_color;
uniform vec3 light_pos;
uniform vec3 view_pos;

void main()
{
    float ambient_strength = 0.2f;
    vec3 ambient_light = light_color * ambient_strength;

    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(light_pos - fragment_position);
    float diff = max(dot(norm, light_dir), 0.0f);
    vec3 diffuse = (diff * light_color);

    float specular_strength = 0.9f;
    vec3 view_dir = normalize(view_pos - fragment_position);
    vec3 reflection_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflection_dir), 0.0), 32);
    vec3 specular = specular_strength * spec * light_color;

    vec3 object_color = (ambient_light + diffuse + specular) * cube_color;
    fragment_color = vec4(object_color, 1.0f);
}