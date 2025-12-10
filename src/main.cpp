#include <iostream>
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SFML/Graphics.hpp>

#include "../headers/shader.h"
#include "../headers/model.h"
#include "../headers/camera.h"
#include "../headers/solar_system.h"

const GLuint SCR_WIDTH = 1280;
const GLuint SCR_HEIGHT = 720;

Camera camera(glm::vec3(0.0f, 0.0f, 150.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;


void processInput(sf::Keyboard::Key key, float dt) {
    if (key == sf::Keyboard::W)
        camera.processKeyboard(FORWARD, dt);
    if (key == sf::Keyboard::S)
        camera.processKeyboard(BACKWARD, dt);
    if (key == sf::Keyboard::E)
        camera.processKeyboard(UP, dt);
    if (key == sf::Keyboard::Q)
        camera.processKeyboard(DOWN, dt);
    if (key == sf::Keyboard::A)
        camera.processKeyboard(LEFT, dt);
    if (key == sf::Keyboard::D)
        camera.processKeyboard(RIGHT, dt);
}

void updateMouseMovement(sf::RenderWindow& window, float xCenter, float yCenter) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    float xpos = (float)mousePos.x;
    float ypos = (float)mousePos.y;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    camera.processMouseMovement(xoffset, yoffset);

    sf::Mouse::setPosition(sf::Vector2i(xCenter, yCenter), window);
    lastX = xCenter;
    lastY = yCenter;
}

int main() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.attributeFlags = sf::ContextSettings::Core;

    sf::RenderWindow window(sf::VideoMode(SCR_WIDTH, SCR_HEIGHT), "Lab13", sf::Style::Default, settings);
    window.setMouseCursorVisible(false);
    window.setFramerateLimit(60);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);


    const char* modelPath = "models/plane.obj";

    Shader* shader = new Shader();
    Model* model = new Model(modelPath);
    SolarSystem* solarSystem = new SolarSystem(shader, model);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 500.0f);
    shader->use();
    shader->setMat4("projection", projection);
    shader->setInt("texture_diffuse", 0);

    sf::Clock clock;
    bool isWindowFocused = true;
    float xCenter = SCR_WIDTH / 2.0f;
    float yCenter = SCR_HEIGHT / 2.0f;

    while (window.isOpen()) {
        float currentFrame = clock.getElapsedTime().asSeconds();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                processInput(event.key.code, deltaTime);
            }

            if (event.type == sf::Event::Resized) {
                glViewport(0, 0, event.size.width, event.size.height);
                projection = glm::perspective(glm::radians(camera.Zoom), (float)event.size.width / (float)event.size.height, 0.1f, 100.0f);
                shader->use();
                shader->setMat4("projection", projection);
            }

            if (event.type == sf::Event::GainedFocus)
                isWindowFocused = true;
            if (event.type == sf::Event::LostFocus) {
                isWindowFocused = false;
                firstMouse = true;
            }
        }

        if (isWindowFocused) {
            updateMouseMovement(window, xCenter, yCenter);
        }

        solarSystem->update(deltaTime);

        glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.getViewMatrix();
        shader->use();
        shader->setMat4("view", view);

        solarSystem->draw();

        window.display();
    }

    delete solarSystem;
    delete model;
    delete shader;

    return 0;
}