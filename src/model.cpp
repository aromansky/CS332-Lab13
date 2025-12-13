#include "../headers/model.h"
#include <fstream>
#include <sstream>
#include <map>
#include <limits> 

Model::~Model() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    if (instanceVBO != 0) {
        glDeleteBuffers(1, &instanceVBO);
    }
}

Model::Model(const char* path) {
    instanceVBO = 0;
    loadModel(path);
    setupMesh();
}

void Model::draw() {
    if (vertices.empty()) {
        std::cerr << "ERROR::MODEL::DRAW: Model vertices are empty. Check OBJ loading." << std::endl;
        return;
    }

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}

void Model::drawInstanced(GLuint instanceCount) {
    if (vertices.empty()) {
        std::cerr << "ERROR::MODEL::DRAW_INSTANCED: Model vertices are empty. Check OBJ loading." << std::endl;
        return;
    }

    if (instanceVBO == 0) {
        std::cerr << "ERROR::MODEL::DRAW_INSTANCED: Instance buffer not setup." << std::endl;
        return;
    }

    glBindVertexArray(VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, vertices.size(), instanceCount);
    glBindVertexArray(0);
}

void Model::setupInstanceBuffer(const std::vector<glm::mat4>& matrices) {
    if (instanceVBO == 0) {
        glGenBuffers(1, &instanceVBO);
    }

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, matrices.size() * sizeof(glm::mat4), matrices.data(), GL_DYNAMIC_DRAW);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

    size_t vec4Size = sizeof(glm::vec4);
    for (int i = 0; i < 4; ++i) {
        glEnableVertexAttribArray(2 + i);
        glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * vec4Size));
        glVertexAttribDivisor(2 + i, 1);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Model::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Model::loadModel(const std::string& path) {
    std::cout << "Loading model from: " << path << std::endl;
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "ERROR::MODEL::LOAD: Failed to open file: " << path << std::endl;
        return;
    }

    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec2> temp_texcoords;

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "v") {
            glm::vec3 position;
            ss >> position.x >> position.y >> position.z;
            temp_positions.push_back(position);
        }
        else if (type == "vt") {
            glm::vec2 texcoord;
            ss >> texcoord.x >> texcoord.y;
            temp_texcoords.push_back(texcoord);
        }
        else if (type == "f") {
            std::string vertex_str;
            std::vector<int> vertex_indices;
            std::vector<int> texcoord_indices;

            while (ss >> vertex_str) {
                size_t pos_v = vertex_str.find('/');
                size_t pos_vt = vertex_str.find('/', pos_v + 1);

                try {
                    int v_idx = std::stoi(vertex_str.substr(0, pos_v));
                    vertex_indices.push_back(v_idx);

                    int vt_idx = std::stoi(vertex_str.substr(pos_v + 1, pos_vt - (pos_v + 1)));
                    texcoord_indices.push_back(vt_idx);
                }
                catch (const std::exception& e) {
                    std::cerr << "ERROR::OBJ::PARSING: Invalid face format in line: " << line << " (" << e.what() << ")" << std::endl;
                    return;
                }
            }

            for (size_t i = 0; i < vertex_indices.size() - 2; ++i) {
                int v_idx_1 = vertex_indices[0];
                int vt_idx_1 = texcoord_indices[0];

                int v_idx_2 = vertex_indices[i + 1];
                int vt_idx_2 = texcoord_indices[i + 1];

                int v_idx_3 = vertex_indices[i + 2];
                int vt_idx_3 = texcoord_indices[i + 2];

                Vertex v1;
                v1.Position = temp_positions[v_idx_1 - 1];
                v1.TexCoords = temp_texcoords[vt_idx_1 - 1];
                vertices.push_back(v1);

                Vertex v2;
                v2.Position = temp_positions[v_idx_2 - 1];
                v2.TexCoords = temp_texcoords[vt_idx_2 - 1];
                vertices.push_back(v2);

                Vertex v3;
                v3.Position = temp_positions[v_idx_3 - 1];
                v3.TexCoords = temp_texcoords[vt_idx_3 - 1];
                vertices.push_back(v3);
            }
        }
    }

    file.close();

    std::cout << "Model loaded successfully. Total vertices (after triangulation/expansion): " << vertices.size() << std::endl;
}