#include "Camera.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(
    glm::vec3 initial_position,
    float sensitivity,
    float speed) : camera_pos(initial_position)
{
    camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
    camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
    world_up = camera_up;

    yaw = 0.0f;
    pitch = -90.0f;
    Camera::sensitivity = sensitivity;
    camera_speed = speed;
}

void Camera::processInput(CameraDirection direction, float delta_time)
{
    const float true_speed = camera_speed * delta_time;

    switch (direction)
    {
    case FORWARD:
        camera_pos += true_speed * camera_front;
        break;
    case BACK:
        camera_pos -= true_speed * camera_front;
        break;
    case LEFT:
        camera_pos -= true_speed * glm::normalize(glm::cross(camera_front, camera_up));
        break;
    case RIGHT:
        camera_pos += true_speed * glm::normalize(glm::cross(camera_front, camera_up));
        break;
    case DOWN:
        camera_pos -= true_speed * camera_up;
        break;
    case UP:
        camera_pos += true_speed * camera_up;
        break;
    default:
        std::cout << "Unknown direction input received." << std::endl;
        break;
    }
}

void Camera::processMouseInput(float xoffset, float yoffset)
{
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera_front = glm::normalize(direction);
}