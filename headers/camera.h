#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// --- Параметры по умолчанию ---
const float YAW = -90.0f; // Начальный угол рысканья
const float PITCH = 0.0f;  // Начальный угол тангажа (горизонтальный взгляд)
const float SPEED = 50.0f;  // Скорость движения камеры
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f; // Угол поля зрения (Field of View, FOV)

class Camera {
public:
    // Свойства камеры
    glm::vec3 Position;  // Позиция камеры в мировых координатах
    glm::vec3 Front;     // Вектор, указывающий направление "вперед" (куда смотрит камера)
    glm::vec3 Up;        // Вектор, указывающий направление "вверх" камеры (локальный)
    glm::vec3 Right;     // Вектор, указывающий направление "вправо" камеры (локальный)
    glm::vec3 WorldUp;   // Вектор глобального "вверх" (обычно (0, 1, 0))

    // Углы Эйлера
    float Yaw;
    float Pitch;

    // Настройки
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom; // FOV

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = YAW,
        float pitch = PITCH);

    /**
     * @brief Возвращает матрицу вида (View Matrix) из glm::lookAt.
     */
    glm::mat4 getViewMatrix();

    /**
     * @brief Обрабатывает ввод с клавиатуры для движения.
     * @param direction Направление движения.
     * @param deltaTime Время, прошедшее с последнего кадра.
     */
    void processKeyboard(Camera_Movement direction, float deltaTime);

    /**
     * @brief Обрабатывает ввод мыши для поворота камеры.
     * @param xoffset Смещение по X (для Yaw).
     * @param yoffset Смещение по Y (для Pitch).
     */
    void processMouseMovement(float xoffset, float yoffset);

private:
    /**
     * @brief Вычисляет векторы Front, Right и Up на основе углов Yaw и Pitch.
     */
    void updateCameraVectors();
};
