/**
 * @file InputManager.cpp
 * @brief Implementación de InputManager
 */

#include "InputManager.hpp"
#include "Grid2D.hpp"
#include "Simulator.hpp"
#include <glm/glm.hpp>

namespace Core {

InputManager* InputManager::instance = nullptr;

InputManager::InputManager(GLFWwindow* window, Renderer::Camera& camera,
                           Grid2D& grid, Simulator& simulator)
    : window(window),
      camera(camera),
      grid(grid),
      simulator(simulator),
      lastX(400.0f),
      lastY(300.0f),
      firstMouse(true),
      mousePressed(false) {
  // Configurar callbacks
  instance = this;
  glfwSetCursorPosCallback(window, mouseCallback);
  glfwSetMouseButtonCallback(window, mouseButtonCallback);
  glfwSetScrollCallback(window, scrollCallback);
}

void InputManager::processKeyboard(float deltaTime) {
  // ESC para cerrar
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  // SPACE para pausar/reanudar (evitar spam con static)
  static bool spacePressed = false;
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !spacePressed) {
    simulator.togglePause();
    spacePressed = true;
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
    spacePressed = false;
  }

  // R para randomizar
  static bool rPressed = false;
  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !rPressed) {
    grid.randomize(0.3f);
    rPressed = true;
  }
  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) {
    rPressed = false;
  }

  // C para limpiar
  static bool cPressed = false;
  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && !cPressed) {
    grid.clear();
    cPressed = true;
  }
  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE) {
    cPressed = false;
  }

  // N para cambiar reglas
  static bool nPressed = false;
  if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && !nPressed) {
    simulator.nextRule();
    nPressed = true;
  }
  if (glfwGetKey(window, GLFW_KEY_N) == GLFW_RELEASE) {
    nPressed = false;
  }

  // WASD para pan
  glm::vec3 movement(0.0f);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    movement.z = 1.0f;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    movement.z = -1.0f;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    movement.x = -1.0f;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    movement.x = 1.0f;
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    movement.y = -1.0f;
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    movement.y = 1.0f;

  if (glm::length(movement) > 0.0f) {
    camera.pan(movement, deltaTime);
  }
}

bool InputManager::shouldClose() const {
  return glfwWindowShouldClose(window);
}

void InputManager::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
  (void)window;

  if (!instance || !instance->mousePressed)
    return;

  if (instance->firstMouse) {
    instance->lastX = xpos;
    instance->lastY = ypos;
    instance->firstMouse = false;
  }

  float xoffset = xpos - instance->lastX;
  float yoffset = instance->lastY - ypos;

  instance->lastX = xpos;
  instance->lastY = ypos;

  instance->camera.processMouseMovement(xoffset, yoffset);
}

void InputManager::mouseButtonCallback(GLFWwindow* window, int button,
                                       int action, int mods) {
  (void)window;
  (void)mods;

  if (!instance)
    return;

  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    if (action == GLFW_PRESS) {
      instance->mousePressed = true;
      instance->firstMouse = true;
    } else if (action == GLFW_RELEASE) {
      instance->mousePressed = false;
    }
  }
}

void InputManager::scrollCallback(GLFWwindow* window, double xoffset,
                                  double yoffset) {
  (void)window;
  (void)xoffset;

  if (instance) {
    instance->camera.processMouseScroll(yoffset);
  }
}

}  // namespace Core
