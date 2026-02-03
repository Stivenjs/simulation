/**
 * @file main.cpp
 * @brief Motor de Simulación 3D - Punto de entrada principal
 *
 * Combina simulación de universo discreto con renderizado 3D en tiempo real.
 */

#include <GL/glew.h>
#include "engine/Window.hpp"
#include <iostream>

int main() {
  try {
    // Crear ventana
    std::cout << "Initializing 3D Simulation Engine..." << std::endl;
    Engine::Window window(800, 600, "3D Universe Simulation - OpenGL");

    std::cout << "Window created successfully!" << std::endl;
    std::cout << "Press ESC or close window to exit." << std::endl;

    // Loop principal
    while (!window.shouldClose()) {
      // Limpiar pantalla con color de fondo
      glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Aquí irá el renderizado de la simulación

      // Intercambiar buffers y procesar eventos
      window.swapBuffers();
      window.pollEvents();
    }

    std::cout << "Shutting down..." << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
