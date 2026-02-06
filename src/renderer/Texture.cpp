/**
 * @file Texture.cpp
 * @brief Implementación de carga de texturas con stb_image
 */

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Texture.hpp"
#include <iostream>

namespace Renderer {

GLuint TextureLoader::loadFromFile(const std::string& filename, const std::string& directory)
{
    std::string fullPath = directory + "/" + filename;
    return loadFromFile(fullPath);
}

GLuint TextureLoader::loadFromFile(const std::string& fullPath)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &channels, 0);

    if (!data) {
        std::cerr << "ERROR::TEXTURE::Failed to load: " << fullPath << std::endl;
        return 0;
    }

    GLuint textureID = generateTexture(data, width, height, channels);
    stbi_image_free(data);

    std::cout << "  Texture loaded: " << fullPath << " (" << width << "x" << height << ", " << channels << "ch)"
              << std::endl;

    return textureID;
}

GLuint TextureLoader::generateTexture(unsigned char* data, int width, int height, int channels)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Determinar formato según número de canales
    GLenum format = GL_RGB;
    if (channels == 1)
        format = GL_RED;
    else if (channels == 3)
        format = GL_RGB;
    else if (channels == 4)
        format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(format), width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parámetros de filtrado y wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}

}  // namespace Renderer
