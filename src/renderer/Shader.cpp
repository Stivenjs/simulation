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
#include <filesystem>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <limits.h>
#endif

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
    // Buscar archivos en múltiples ubicaciones
    std::string vertexFile = findFile(vertexPath);
    std::string fragmentFile = findFile(fragmentPath);

    // Leer archivos
    std::string vertexSource = readFile(vertexFile);
    std::string fragmentSource = readFile(fragmentFile);

    // Crear y devolver unique_ptr al shader
    return std::make_unique<Shader>(vertexSource, fragmentSource);
}

std::string Shader::findFile(const std::string& filepath)
{
    namespace fs = std::filesystem;

    // Lista de ubicaciones donde buscar
    std::vector<std::string> searchPaths;

    // 1. Ruta exacta proporcionada
    searchPaths.push_back(filepath);

    // 2. Directorio actual de trabajo
    try {
        std::string currentDir = fs::current_path().string();
        // Normalizar separadores de ruta para Windows
        std::replace(currentDir.begin(), currentDir.end(), '\\', '/');
        searchPaths.push_back(currentDir + "/" + filepath);
    } catch (...) {
        // Ignorar errores al obtener directorio actual
    }

    // 3. Obtener directorio del ejecutable
    std::string exeDir;
#ifdef _WIN32
    char exePath[MAX_PATH];
    DWORD result = GetModuleFileNameA(nullptr, exePath, MAX_PATH);
    if (result > 0 && result < MAX_PATH) {
        fs::path exePathObj(exePath);
        exeDir = exePathObj.parent_path().string();
        // Normalizar separadores
        std::replace(exeDir.begin(), exeDir.end(), '\\', '/');
    }
#else
    char exePath[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", exePath, PATH_MAX);
    if (count != -1 && count < PATH_MAX) {
        exePath[count] = '\0';
        exeDir = fs::path(exePath).parent_path().string();
    }
#endif

    if (!exeDir.empty()) {
        // 4. Directorio del ejecutable + ruta relativa
        searchPaths.push_back(exeDir + "/" + filepath);

        // 5. Directorio del ejecutable + ../shaders/...
        if (filepath.find("shaders/") == 0 || filepath.find("shaders\\") == 0) {
            searchPaths.push_back(exeDir + "/../" + filepath);
            searchPaths.push_back(exeDir + "/../../" + filepath);
        } else {
            std::string filename = fs::path(filepath).filename().string();
            searchPaths.push_back(exeDir + "/../shaders/" + filename);
            searchPaths.push_back(exeDir + "/../../shaders/" + filename);
        }
    }

    // Buscar el primer archivo que exista
    for (const auto& path : searchPaths) {
        try {
            // Intentar con separadores normales
            if (fs::exists(path) && fs::is_regular_file(path)) {
                return path;
            }
            // Intentar con separadores de Windows si estamos en Windows
#ifdef _WIN32
            std::string winPath = path;
            std::replace(winPath.begin(), winPath.end(), '/', '\\');
            if (fs::exists(winPath) && fs::is_regular_file(winPath)) {
                return winPath;
            }
#endif
        } catch (...) {
            // Continuar buscando en otras ubicaciones
            continue;
        }
    }

    // Si no se encuentra, devolver la ruta original (fallará con mensaje de error claro)
    return filepath;
}

std::string Shader::readFile(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open shader file: " + filepath
                                 + "\nMake sure the shaders directory exists relative to the executable.");
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

void Shader::setBool(const std::string& name, bool value) const
{
    GLint location = glGetUniformLocation(program, name.c_str());
    glUniform1i(location, static_cast<int>(value));
}

void Shader::setInt(const std::string& name, int value) const
{
    GLint location = glGetUniformLocation(program, name.c_str());
    glUniform1i(location, value);
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
