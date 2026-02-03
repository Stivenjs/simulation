/**
 * @file main.cpp
 * @brief Motor de Simulación 3D - Renderizado de Cubo
 *
 * Combina simulación de universo discreto con renderizado 3D en tiempo real.
 */

#include <GL/glew.h>
#include "engine/Window.hpp"
#include "renderer/Shader.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main() {
  try {
    // Crear ventana
    std::cout << "Initializing 3D Simulation Engine..." << std::endl;
    Engine::Window window(800, 600, "3D Universe Simulation - Rotating Cube");

    std::cout << "Loading shaders from files..." << std::endl;
    Renderer::Shader shader =
        Renderer::Shader::fromFiles("shaders/basic.vert", "shaders/basic.frag");

    // Definir vértices del cubo con colores
    // Cada cara tiene un color diferente
    float vertices[] = {// Posiciones        // Colores (cada cara un color)
                        // Cara frontal (rojo)
                        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.5f,
                        1.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
                        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

                        // Cara trasera (verde)
                        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.5f, -0.5f,
                        -0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
                        0.0f, -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,

                        // Cara superior (azul)
                        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f, -0.5f,
                        0.0f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
                        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

                        // Cara inferior (amarillo)
                        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.5f, -0.5f,
                        -0.5f, 1.0f, 1.0f, 0.0f, 0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
                        0.0f, -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f,

                        // Cara derecha (magenta)
                        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.5f, 0.5f, -0.5f,
                        1.0f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
                        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f,

                        // Cara izquierda (cyan)
                        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, -0.5f, 0.5f,
                        -0.5f, 0.0f, 1.0f, 1.0f, -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
                        1.0f, -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f};

    // Índices para dibujar el cubo (2 triángulos por cara = 6 caras * 6
    // vértices = 36)
    unsigned int indices[] = {// Frontal
                              0, 1, 2, 2, 3, 0,
                              // Trasera
                              4, 6, 5, 6, 4, 7,
                              // Superior
                              8, 9, 10, 10, 11, 8,
                              // Inferior
                              12, 14, 13, 14, 12, 15,
                              // Derecha
                              16, 17, 18, 18, 19, 16,
                              // Izquierda
                              20, 22, 21, 22, 20, 23};

    // Crear VAO, VBO, EBO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // VBO - Vértices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // EBO - Índices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    // Atributos de vértices
    // Posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Habilitar depth testing para ver caras correctamente
    glEnable(GL_DEPTH_TEST);

    std::cout << "Cube ready! Starting render loop..." << std::endl;
    std::cout << "Press ESC or close window to exit." << std::endl;

    // Variables para animación
    float lastFrame = 0.0f;
    float currentFrame = 0.0f;
    float deltaTime = 0.0f;
    float rotation = 0.0f;

    // Loop principal
    while (!window.shouldClose()) {
      // Calcular delta time
      currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;

      // Actualizar rotación
      rotation += 45.0f * deltaTime;  // 45 grados por segundo
      if (rotation > 360.0f)
        rotation -= 360.0f;

      // Limpiar pantalla y depth buffer
      glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Activar shader
      shader.use();

      // Matriz de modelo (rotación del cubo)
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::rotate(model, glm::radians(rotation),
                          glm::vec3(0.5f, 1.0f, 0.0f));

      // Matriz de vista (cámara)
      glm::mat4 view = glm::mat4(1.0f);
      view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

      // Matriz de proyección (perspectiva)
      glm::mat4 projection = glm::perspective(
          glm::radians(45.0f),
          (float)window.getWidth() / (float)window.getHeight(), 0.1f, 100.0f);

      // Enviar matrices al shader
      shader.setMat4("model", glm::value_ptr(model));
      shader.setMat4("view", glm::value_ptr(view));
      shader.setMat4("projection", glm::value_ptr(projection));

      // Renderizar cubo
      glBindVertexArray(VAO);
      glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);

      shader.unuse();

      // Intercambiar buffers y procesar eventos
      window.swapBuffers();
      window.pollEvents();
    }

    // Limpiar recursos
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    std::cout << "Shutting down..." << std::endl;

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
