/**
 * @file main.cpp
 * @brief Motor de Simulación 3D - Punto de entrada principal
 *
 * Combina simulación de universo discreto con renderizado 3D en tiempo real.
 */

#include "renderer/Shader.hpp"
#include "engine/Window.hpp"
#include <GL/glew.h>
#include <iostream>


int main() {
  try {
    // Crear ventana
    std::cout << "Initializing 3D Simulation Engine..." << std::endl;
    Engine::Window window(800, 600, "3D Universe Simulation - Triangle Test");

    std::cout << "Loading shaders from files..." << std::endl;
    Renderer::Shader shader =
        Renderer::Shader::fromFiles("shaders/basic.vert", "shaders/basic.frag");

    // Definir vértices del triángulo (posición + color)
    float vertices[] = {
        // Posiciones        // Colores
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.0f, // Vértice inferior izquierdo (rojo)
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f,
        0.0f,                                // Vértice inferior derecho (verde)
        0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f // Vértice superior (azul)
    };

    // Crear VAO y VBO
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bindear VAO
    glBindVertexArray(VAO);

    // Bindear y configurar VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configurar atributos de vértices
    // Posición (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    // Color (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbindear
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    std::cout << "Triangle ready! Starting render loop..." << std::endl;
    std::cout << "Press ESC or close window to exit." << std::endl;

    // Loop principal
    while (!window.shouldClose()) {
      // Limpiar pantalla
      glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Usar shader y renderizar triángulo
      shader.use();
      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glBindVertexArray(0);
      shader.unuse();

      // Intercambiar buffers y procesar eventos
      window.swapBuffers();
      window.pollEvents();
    }

    // Limpiar recursos
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    std::cout << "Shutting down..." << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
