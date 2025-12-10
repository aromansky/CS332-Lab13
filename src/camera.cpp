#include "../headers/camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cmath>

/**
 * @brief Инициализирует камеру.
 * * @param position Начальная позиция.
 * @param up Вектор "вверх" мировых координат.
 * @param yaw Начальный угол рысканья.
 * @param pitch Начальный угол тангажа.
 */
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
// Инициализация полей
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), // Изначально смотрим вдоль отрицательной оси Z
    MovementSpeed(SPEED),
    MouseSensitivity(SENSITIVITY),
    Zoom(ZOOM) {

    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors(); // Вычисляем векторы Front, Right и Up на основе углов
}

/**
 * @brief Возвращает View Matrix для передачи в шейдер.
 * * Матрица вида преобразует мировые координаты в координаты вида камеры.
 * glm::lookAt(позиция_камеры, позиция_цели, вектор_вверх_камеры)
 */
glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
}

/**
 * @brief Обрабатывает движение камеры
 * * Движение по осям (вперед/назад/вверх/вниз) зависит от локальных векторов камеры (Front, WorldUp).
 * * @param direction Направление движения (FORWARD, BACKWARD, UP, DOWN).
 * @param deltaTime Время, прошедшее с последнего кадра, для корректной скорости.
 */
void Camera::processKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;

    if (direction == FORWARD)
        Position += Front * velocity;

    if (direction == BACKWARD)
        Position -= Front * velocity;

    // Движение вверх/вниз относительно мировых координат (или WorldUp, если хотите "летать" ровно)
    if (direction == UP)
        Position += WorldUp * velocity;

    if (direction == DOWN)
        Position -= WorldUp * velocity;

    if (direction == RIGHT)
        Position += Right * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
}

/**
 * @brief Обрабатывает поворот камеры на основе смещения курсора мыши.
 * * Изменяет углы Yaw и Pitch, затем обновляет векторы камеры.
 * * @param xoffset Смещение мыши по X (для Yaw).
 * @param yoffset Смещение мыши по Y (для Pitch).
 */
void Camera::processMouseMovement(float xoffset, float yoffset) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // Ограничение угла наклона (Pitch), чтобы избежать "кувыркания"
    if (Pitch > 89.0f)
        Pitch = 89.0f;
    if (Pitch < -89.0f)
        Pitch = -89.0f;

    updateCameraVectors();
}

/**
 * @brief Вычисляет векторы Front, Right и Up на основе углов Yaw и Pitch
 */
void Camera::updateCameraVectors() {
    glm::vec3 front;

    // Вычисление нового вектора Front
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    Front = glm::normalize(front);

    // Пересчет вектора Right (векторное произведение Front и WorldUp)
    Right = glm::normalize(glm::cross(Front, WorldUp));

    // Пересчет вектора Up (векторное произведение Right и Front)
    // Это гарантирует, что Up перпендикулярен Front и Right
    Up = glm::normalize(glm::cross(Right, Front));
}