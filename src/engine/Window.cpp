/**
 * @file Window.cpp
 * @brief Implementación de la clase Window
 */

#include <GL/glew.h>
#include "Window.hpp"
#include <iostream>
#include <stdexcept>

namespace Engine {

Window::Window(int width, int height, const std::string& title) :
    window(nullptr), width(width), height(height), title(title), displayMode(DisplayMode::Windowed),
    windowedWidth(width), windowedHeight(height), windowedPosX(100), windowedPosY(100)
{
    init();
}

Window::~Window()
{
    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

void Window::init()
{
    // Inicializar GLFW
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    // Configurar OpenGL 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Crear ventana
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    // Hacer el contexto OpenGL actual
    glfwMakeContextCurrent(window);

    // Inicializar GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        throw std::runtime_error("Failed to initialize GLEW");
    }

    // Configurar viewport
    glViewport(0, 0, width, height);

    // Configurar callback para redimensionamiento
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Habilitar V-Sync
    glfwSwapInterval(1);

    // Información de OpenGL
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(window);
}

void Window::pollEvents()
{
    glfwPollEvents();
}

void Window::swapBuffers()
{
    glfwSwapBuffers(window);
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}

void Window::setCursorCaptured(bool captured)
{
    if (captured) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

std::vector<Window::Resolution> Window::getAvailableResolutions() const
{
    std::vector<Resolution> resolutions;

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (!monitor) {
        return resolutions;
    }

    int count;
    const GLFWvidmode* modes = glfwGetVideoModes(monitor, &count);

    // Agregar resoluciones únicas
    for (int i = 0; i < count; i++) {
        Resolution res { modes[i].width, modes[i].height };

        // Evitar duplicados
        bool exists = false;
        for (const auto& existing : resolutions) {
            if (existing == res) {
                exists = true;
                break;
            }
        }

        if (!exists) {
            resolutions.push_back(res);
        }
    }

    return resolutions;
}

void Window::setDisplayMode(int newWidth, int newHeight, DisplayMode mode)
{
    width = newWidth;
    height = newHeight;
    displayMode = mode;

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();

    switch (mode) {
    case DisplayMode::Fullscreen:
        glfwSetWindowMonitor(window, monitor, 0, 0, width, height, GLFW_DONT_CARE);
        break;

    case DisplayMode::BorderlessFullscreen: {
        const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(window, monitor, 0, 0, videoMode->width, videoMode->height, videoMode->refreshRate);
        width = videoMode->width;
        height = videoMode->height;
        break;
    }

    case DisplayMode::Windowed:
        // Guardar posición actual si estamos en fullscreen
        if (displayMode != DisplayMode::Windowed) {
            windowedPosX = 100;
            windowedPosY = 100;
        }

        glfwSetWindowMonitor(window, nullptr, windowedPosX, windowedPosY, width, height, GLFW_DONT_CARE);
        windowedWidth = width;
        windowedHeight = height;
        break;
    }

    // Actualizar viewport
    glViewport(0, 0, width, height);
}

void Window::setFullscreen()
{
    setDisplayMode(width, height, DisplayMode::Fullscreen);
}

void Window::setBorderlessFullscreen()
{
    setDisplayMode(width, height, DisplayMode::BorderlessFullscreen);
}

void Window::setWindowed()
{
    setDisplayMode(windowedWidth, windowedHeight, DisplayMode::Windowed);
}

}  // namespace Engine
