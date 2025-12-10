#pragma once

#include <GL/glew.h>  // Для GLuint
#include <glm/glm.hpp> // Для glm::mat4, glm::vec3
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

/**
 * @brief Класс для управления шейдерной программой OpenGL.
 * * * Отвечает за:
 * 1. Чтение исходного кода шейдеров из файлов (.vert, .frag).
 * 2. Компиляцию и линковку шейдеров в шейдерную программу.
 * 3. Активацию программы (glUseProgram).
 * 4. Установку значений переменных-униформ (uniforms).
 */
class Shader {
public:
    // ID шейдерной программы, используемый OpenGL
    GLuint ID;

    /**
     * Конструктор: создает шейдерную программу, используя встроенные строки.
     * vertexPath Игнорируется, но сохранен для совместимости.
     * fragmentPath Игнорируется, но сохранен для совместимости.
     */
    Shader();

    /**
     *Деструктор: удаляет шейдерную программу из памяти GPU.
     */
    ~Shader();

    
     // Активирует шейдерную программу для использования.
    void use();

    // --- Методы для установки униформ ---

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;

    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;

    void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
    /**
     * @brief Вспомогательная функция для проверки ошибок компиляции/линковки.
     * @param shader ID шейдера или программы.
     * @param type Тип ("VERTEX", "FRAGMENT" или "PROGRAM").
     */
    void checkCompileErrors(GLuint shader, std::string type);
};