#include "../headers/texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" 

Texture::Texture(const char* path) : width(0), height(0), nrChannels(0), ID(0) {
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

    if (data) {
        GLenum format = GL_RGB;
        if (nrChannels == 4)
            format = GL_RGBA;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 1)
            format = GL_RED;
        else {
            std::cerr << "WARNING::TEXTURE::FORMAT: Unsupported number of channels (" << nrChannels << ") for: " << path << std::endl;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "ERROR::TEXTURE::LOAD: Failed to load texture at path: " << path << std::endl;
    }

    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    glDeleteTextures(1, &ID);
}

void Texture::bind(GLuint unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}