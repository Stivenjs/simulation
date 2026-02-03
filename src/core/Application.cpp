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

  // Crear UI (después de crear ventana)
  ui = std::make_unique<Renderer::UI>(window->getHandle());

  // Nota: InputManager se crea después del grid y simulator

  // Crear grid de simulación (20x20)
  grid = std::make_unique<Grid2D>(20, 20);
  grid->randomize(0.3f);  // 30% de celdas vivas

  // Crear simulador
  simulator = std::make_unique<Simulator>(*grid);
  simulator->setSpeed(5.0f);  // 5 pasos por segundo

  // Crear estadísticas
  stats = std::make_unique<Stats>();

  // Crear mesh del cubo (reutilizable para cada celda)
  cubeMesh = Renderer::Mesh::createCube();

  // Crear input manager (después de grid y simulator)
  inputManager = std::make_unique<InputManager>(window->getHandle(), *camera,
                                                *grid, *simulator);

  // Configurar OpenGL
  glEnable(GL_DEPTH_TEST);

  std::cout << "\nGrid initialized: " << grid->getWidth() << "x"
            << grid->getHeight() << std::endl;
  std::cout << "Simulation: Game of Life (Conway)" << std::endl;
  std::cout << "\nControls:" << std::endl;
  std::cout << "  SPACE: Pause/Resume simulation" << std::endl;
  std::cout << "  R: Randomize grid" << std::endl;
  std::cout << "  C: Clear grid" << std::endl;
  std::cout << "  N: Next rule set" << std::endl;
  std::cout << "  Mouse Left + Drag: Rotate camera" << std::endl;
  std::cout << "  Mouse Scroll: Zoom in/out" << std::endl;
  std::cout << "  WASD: Pan camera | Q/E: Up/Down" << std::endl;
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
  simulator->update(deltaTime);
  stats->update(*grid, deltaTime);

  // Mostrar stats cada segundo
  static float statsTimer = 0.0f;
  statsTimer += deltaTime;
  if (statsTimer >= 1.0f) {
    std::cout << "\r" << Rules::getName(simulator->getRuleType())
              << " | Gen: " << simulator->getGeneration() << " | "
              << stats->toString() << "          " << std::flush;
    statsTimer = 0.0f;
  }
}

void Application::render() {
  // Limpiar pantalla
  glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shader->use();

  // Matriz de vista desde la cámara
  glm::mat4 view = camera->getViewMatrix();

  // Matriz de proyección (usar tamaño actual de la ventana)
  int currentWidth = window->getWidth();
  int currentHeight = window->getHeight();
  glm::mat4 projection = glm::perspective(
      glm::radians(45.0f), (float)currentWidth / (float)currentHeight, 0.1f,
      100.0f);

  shader->setMat4("view", glm::value_ptr(view));
  shader->setMat4("projection", glm::value_ptr(projection));

  // Renderizar grid - un cubo por cada celda
  float spacing = 1.2f;  // Espacio entre cubos
  float gridOffsetX = -(grid->getWidth() * spacing) / 2.0f;
  float gridOffsetZ = -(grid->getHeight() * spacing) / 2.0f;

  for (int y = 0; y < grid->getHeight(); ++y) {
    for (int x = 0; x < grid->getWidth(); ++x) {
      CellState state = grid->getCell(x, y);

      // Solo renderizar celdas vivas
      if (state == CellState::ALIVE) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(gridOffsetX + x * spacing, 0.0f,
                                                gridOffsetZ + y * spacing));
        model = glm::scale(model, glm::vec3(0.5f));  // Cubos más pequeños

        // Obtener color dinámico basado en vecinos
        float r, g, b;
        grid->getCellColor(x, y, r, g, b);

        shader->setMat4("model", glm::value_ptr(model));
        shader->setVec3("cellColor", r, g, b);
        cubeMesh->draw();
      }
    }
  }

  shader->unuse();

  // Renderizar UI (debe ser después de la escena 3D)
  ui->newFrame();
  ui->renderStatsPanel(*simulator, *stats, *grid);
  ui->renderVideoSettingsPanel(*window);
  ui->render();
}

void Application::calculateDeltaTime() {
  float currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;
}

}  // namespace Core