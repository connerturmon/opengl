#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
    Camera();

    glm::vec3 position() const { return camera_pos; }
    glm::vec3 front() const { return camera_front; }
    glm::vec3 up() const { return camera_up; }
    float getYaw() const { return yaw; }
    float getPitch() const { return pitch; }

    void setPos();
    void setFront();
    void setUp();
    void setYaw();
    void setPitch();
    
private:
    glm::vec3 camera_pos;
    glm::vec3 camera_front;
    glm::vec3 camera_up;
    float yaw, pitch;
};