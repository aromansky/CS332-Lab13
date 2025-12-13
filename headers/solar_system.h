#pragma once

#include "../headers/shader.h"
#include "../headers/model.h"
#include "../headers/texture.h"

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct CelestialBody {
    glm::vec3 Position;
    float OrbitRadius;
    float OrbitSpeed;
    float RotationSpeed;
    float Scale;
    float OrbitAngle;
    float RotationAngle;

    Texture* texture;
    glm::mat4 ModelMatrix;
};


class SolarSystem {
public:

    SolarSystem(Shader* shader, Model* model);

    ~SolarSystem();

    void update(float deltaTime);

    void draw();

private:
    Shader* m_shader;
    Model* m_model;

    CelestialBody m_sun;
    std::vector<CelestialBody> m_planets;
    
    std::vector<glm::mat4> instanceMatrices;

    void initializeSystem();
};
