#pragma once

#include <glad/glad.h>

class Texture
{
public:
    Texture(const char *path);
    ~Texture() { glBindTexture(GL_TEXTURE_2D, 0); }

    void bind() { glBindTexture(GL_TEXTURE_2D, id); }
    
private:
    GLuint id;
    const char *path;
    int width;
    int height;
    int n_channels;
    uint8_t *texture_data;
};