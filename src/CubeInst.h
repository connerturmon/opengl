#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "ShaderProgram.h"

class CubeInst
{
public:
    CubeInst(glm::vec3 _pos, glm::vec3 _color, float _scale, ShaderProgram _program, GLuint _vao)
        : position(_pos), color(_color), scale(_scale), program(_program), vao(_vao) {};

    void Render();

private:
    glm::vec3 position;
    glm::vec3 color;
    float scale;
    ShaderProgram program;
    GLuint vao;
};