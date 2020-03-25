#include "Cube.h"

GLuint Cube::vbo;

Cube::Cube()
{
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);

    // Unbind buffers after initialization.
    // Be sure to rebind before calling draw functions.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Cube::~Cube()
{
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Cube::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}