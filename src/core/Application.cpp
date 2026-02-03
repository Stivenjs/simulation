/**
 * @file Application.cpp
 * @brief Implementación de Application
 */

#include <GL/glew.h>
#include "Application.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Core {

Application::Application(int width, int height, const std::string& title)
    : width(width),
      height(height),
      title(title),
      lastFrame(0.0f),
      deltaTime(0.0f) {}

void Application::init() {
  std::cout << "Initializing 3D Simulation Engine..." << std::endl;

  // Crear ventana
  window = std::make_unique<Engine::Window>(width, height, title);

  // Cargar shaders
  std::cout << "Loading shaders..." << std::endl;
  shader =
      Renderer::Shader::fromFiles("shaders/basic.vert", "shaders/basic.frag");

  // Crear cámara
  camera = std::make_unique<Renderer::Camera>(glm::vec3(0.0f, 2.0f, 5.0f));

  // Crear input manager
  inputManager = std::make_unique<InputManager>(window->getHandle(), *camera);

  // Crear cubo
  meshes.push_back(Renderer::Mesh::createCube());

  // Configurar OpenGL
  glEnable(GL_DEPTH_TEST);

  std::cout << "\nControls:" << std::endl;
  std::cout << "  Mouse Left Button + Drag: Rotate camera" << std::endl;
  std::cout << "  Mouse Scroll: Zoom in/out" << std::endl;
  std::cout << "  WASD: Pan camera" << std::endl;
  std::cout << "  Q/E: Move up/down" << std::endl;
  std::cout << "  ESC: Exit\n" << std::endl;
}

void Application::run() {
  while (!window->shouldClose()) {
    calculateDeltaTime();
    update();
    render();
    window->swapBuffers();
    window->pollEvents();
  }

  std::cout << "Shutting down..." << std::endl;
}

void Application::update() {
  inputManager->processKeyboard(deltaTime);
}

void Application::render() {
  // Limpiar pantalla
  glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shader->use();

  // Matriz de vista desde la cámara
  glm::mat4 view = camera->getViewMatrix();

  // Matriz de proyección
  glm::mat4 projection = glm::perspective(
      glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

  shader->setMat4("view", glm::value_ptr(view));
  shader->setMat4("projection", glm::value_ptr(projection));

  // Renderizar todos los meshes
  for (const auto& mesh : meshes) {
    shader->setMat4("model", glm::value_ptr(mesh->getModelMatrix()));
    mesh->draw();
  }

  shader->unuse();
}

void Application::calculateDeltaTime() {
  float currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;
}

}  // namespace Core
