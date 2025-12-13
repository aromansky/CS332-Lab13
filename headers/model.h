#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <iostream>


struct Vertex {
    glm::vec3 Position;
    glm::vec2 TexCoords;
};


class Model {
public:
    
    Model(const char* path);

    ~Model();

    void draw();
    void drawInstanced(GLuint instanceCount);
    void setupInstanceBuffer(const std::vector<glm::mat4>& matrices);

private:
    GLuint VAO, VBO, instanceVBO;

    std::vector<Vertex> vertices;

    void loadModel(const std::string& path);
    void setupMesh();

    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;
};