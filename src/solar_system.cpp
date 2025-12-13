#include "../headers/solar_system.h"
#include <iostream>
#include <cmath>

SolarSystem::SolarSystem(Shader* shader, Model* model)
    : m_shader(shader), m_model(model) {
    initializeSystem();
    instanceMatrices.resize(m_planets.size());
}

SolarSystem::~SolarSystem() {
    delete m_sun.texture;
    for (const auto& p : m_planets) {
        delete p.texture;
    }
    m_planets.clear();
}

void SolarSystem::initializeSystem() {
    std::cout << "Initializing Solar System with 6 bodies..." << std::endl;

    m_sun.OrbitRadius = 0.0f;
    m_sun.OrbitSpeed = 0.0f;
    m_sun.RotationSpeed = 15.0f;
    m_sun.Scale = 1.5f;
    m_sun.RotationAngle = 0.0f;
    m_sun.OrbitAngle = 0.0f;
    m_sun.Position = glm::vec3(0.0f);
    m_sun.texture = new Texture("textures/sun_tex.png");

    float baseRadius = 5.0f;
    float radiusIncrement = 15.0f;
    float baseSpeed = 40.0f;
    float baseScale = 0.3f;

    for (int i = 0; i < 5; ++i) {
        CelestialBody p;
        p.OrbitRadius = baseRadius + i * radiusIncrement;
        p.OrbitSpeed = baseSpeed / (1.0f + (float)i * 0.5f);
        p.RotationSpeed = 50.0f + i * 15.0f;
        p.Scale = baseScale + i * 0.15f;

        p.OrbitAngle = (float)(i * 72.0f); 
        p.RotationAngle = 0.0f;
        p.Position = glm::vec3(0.0f);

        std::string texPath = "textures/planet_tex.png";
        p.texture = new Texture(texPath.c_str());

        m_planets.push_back(p);
    }
}

void SolarSystem::update(float deltaTime) {
    m_sun.RotationAngle += m_sun.RotationSpeed * deltaTime;

    m_sun.ModelMatrix = glm::mat4(1.0f);
    m_sun.ModelMatrix = glm::scale(m_sun.ModelMatrix, glm::vec3(m_sun.Scale));
    m_sun.ModelMatrix = glm::rotate(m_sun.ModelMatrix, glm::radians(m_sun.RotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    m_sun.ModelMatrix = glm::rotate(m_sun.ModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    for (size_t i = 0; i < m_planets.size(); ++i) {
        CelestialBody& p = m_planets[i];
        p.OrbitAngle += p.OrbitSpeed * deltaTime;

        p.Position.x = p.OrbitRadius * cos(glm::radians(p.OrbitAngle));
        p.Position.z = p.OrbitRadius * sin(glm::radians(p.OrbitAngle));

        p.RotationAngle += p.RotationSpeed * deltaTime;
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, p.Position);
        model = glm::rotate(model, glm::radians(p.RotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(p.Scale));

        p.ModelMatrix = model;
        instanceMatrices[i] = model;
    }

    m_model->setupInstanceBuffer(instanceMatrices);
}

void SolarSystem::draw() {
    m_shader->use();

    // Рендеринг Солнца
    m_sun.texture->bind(0);
    m_shader->setMat4("model", m_sun.ModelMatrix);
    m_shader->setBool("useInstanceMatrix", false);
    m_model->draw();

    // Рендеринг планет
    m_shader->setBool("useInstanceMatrix", true);
    m_model->setupInstanceBuffer(instanceMatrices);
    
    for (size_t i = 0; i < m_planets.size(); ++i) {
        m_planets[i].texture->bind(0);
    }

    m_model->drawInstanced(static_cast<GLuint>(m_planets.size()));
}