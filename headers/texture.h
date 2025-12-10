#pragma once

#include <GL/glew.h>
#include <string>
#include <iostream>


class Texture {
public:
    GLuint ID;

    Texture(const char* path);

    ~Texture();

    void bind(GLuint unit);

    void unbind();

private:
    int width;
    int height;
    int nrChannels;

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
};
