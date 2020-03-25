#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraDirection
{
    FORWARD,
    BACK,
    LEFT,
    RIGHT,
    DOWN,
    UP
};

class Camera
{
public:
    Camera(
        glm::vec3 initial_position = glm::vec3(0.0f, 0.0f, 0.0f),
        float sensitivity = 0.1f,
        float speed = 2.0f);

    glm::vec3 getPosition() const { return camera_pos; }
    glm::vec3 getFront() const { return camera_front; }
    glm::vec3 getUp() const { return camera_up; }
    float getYaw() const { return yaw; }
    float getPitch() const { return pitch; }

    void setSensitivty(float sensitivity) { Camera::sensitivity = sensitivity; }
    void setCamSpeed(float speed) { camera_speed = speed; }

    glm::mat4 viewMatrix() const
    {
        return glm::lookAt(
            camera_pos,
            camera_pos + camera_front,
            camera_up);
    }

    void processInput(CameraDirection direction, float delta_time);
    void processMouseInput(float xoffset, float yoffset);

private:
    glm::vec3 camera_pos;
    glm::vec3 camera_front;
    glm::vec3 camera_up;
    glm::vec3 world_up;
    float yaw, pitch;
    float sensitivity;
    float camera_speed;
};