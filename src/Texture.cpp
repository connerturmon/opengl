#include "Texture.h"

#include <iostream>
#include <stb_image.h>

Texture::Texture(const char *path) : path(path)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    texture_data = stbi_load(
        path, &width, &height, &n_channels, 0);

    if (texture_data)
    {
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGB, width, 
            height, 0, GL_RGB, GL_UNSIGNED_BYTE, 
            texture_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "STB_IMAGE Error: Failed to read texture."
            << std::endl;

    stbi_image_free(texture_data);
    glBindTexture(GL_TEXTURE_2D, 0);
}