/**
 * @file Shader.cpp
 * @brief Implementación de la clase Shader
 */

#include "Shader.hpp"
#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace Renderer {

Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource)
{
    // Compilar shaders
    GLuint vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

    // Enlazar programa
    linkProgram(vertexShader, fragmentShader);

    // Eliminar shaders (ya están linkeados en el programa)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

std::unique_ptr<Shader> Shader::fromFiles(const std::string& vertexPath, const std::string& fragmentPath)
{
    // Leer archivos
    std::string vertexSource = readFile(vertexPath);
    std::string fragmentSource = readFile(fragmentPath);

    // Crear y devolver unique_ptr al shader
    return std::make_unique<Shader>(vertexSource, fragmentSource);
}

std::string Shader::readFile(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open shader file: " + filepath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

Shader::~Shader()
{
    if (program != 0) {
        glDeleteProgram(program);
    }
}

void Shader::use() const
{
    glUseProgram(program);
}

void Shader::unuse() const
{
    glUseProgram(0);
}

GLuint Shader::compileShader(const std::string& source, GLenum type)
{
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    // Verificar errores
    std::string typeStr = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
    checkCompileErrors(shader, typeStr);

    return shader;
}

void Shader::linkProgram(GLuint vertexShader, GLuint fragmentShader)
{
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Verificar errores
    checkCompileErrors(program, "PROGRAM");
}

void Shader::checkCompileErrors(GLuint shader, const std::string& type)
{
    GLint success;
    std::vector<GLchar> infoLog(1024);

    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog.data());
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog.data() << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog.data());
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog.data() << std::endl;
        }
    }
}

void Shader::setFloat(const std::string& name, float value) const
{
    GLint location = glGetUniformLocation(program, name.c_str());
    glUniform1f(location, value);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
    GLint location = glGetUniformLocation(program, name.c_str());
    glUniform3f(location, x, y, z);
}

void Shader::setMat4(const std::string& name, const float* matrix) const
{
    GLint location = glGetUniformLocation(program, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, matrix);
}

}  // namespace Renderer
