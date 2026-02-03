/**
 * @file Window.cpp
 * @brief Implementación de la clase Window
 */

#include <GL/glew.h>
#include "Window.hpp"
#include <iostream>
#include <stdexcept>

namespace Engine {

Window::Window(int width, int height, const std::string& title)
    : window(nullptr), width(width), height(height), title(title) {
  init();
}

Window::~Window() {
  if (window) {
    glfwDestroyWindow(window);
  }
  glfwTerminate();
}

void Window::init() {
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
  std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION)
            << std::endl;
}

bool Window::shouldClose() const {
  return glfwWindowShouldClose(window);
}

void Window::pollEvents() {
  glfwPollEvents();
}

void Window::swapBuffers() {
  glfwSwapBuffers(window);
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width,
                                     int height) {
  (void)window;
  glViewport(0, 0, width, height);
}

void Window::setCursorCaptured(bool captured) {
  if (captured) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  } else {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
}

}  // namespace Engine
