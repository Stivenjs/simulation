/**
 * @file Window.hpp
 * @brief Gestión de ventana con GLFW
 *
 * Encapsula la creación y manejo de una ventana GLFW
 * para renderizado con OpenGL.
 */

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <GLFW/glfw3.h>
#include <string>

namespace Engine {

/**
 * @class Window
 * @brief Clase para gestionar la ventana de renderizado
 */
class Window {
 public:
  /**
   * @brief Constructor
   * @param width Ancho de la ventana en píxeles
   * @param height Alto de la ventana en píxeles
   * @param title Título de la ventana
   */
  Window(int width, int height, const std::string& title);

  /**
   * @brief Destructor - Limpia recursos de GLFW
   */
  ~Window();

  // Prevenir copia
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;

  /**
   * @brief Verifica si la ventana debe cerrarse
   * @return true si debe cerrarse, false en caso contrario
   */
  bool shouldClose() const;

  /**
   * @brief Procesa eventos de entrada (teclado, mouse, etc.)
   */
  void pollEvents();

  /**
   * @brief Intercambia buffers (muestra el frame renderizado)
   */
  void swapBuffers();

  /**
   * @brief Obtiene el puntero a la ventana GLFW
   * @return Puntero a GLFWwindow
   */
  GLFWwindow* getHandle() const {
    return window;
  }

  /**
   * @brief Obtiene el ancho actual de la ventana
   * @return Ancho en píxeles
   */
  int getWidth() const {
    return width;
  }

  /**
   * @brief Obtiene el alto actual de la ventana
   * @return Alto en píxeles
   */
  int getHeight() const {
    return height;
  }

  /**
   * @brief Establece si el cursor está capturado
   * @param captured true para capturar, false para liberar
   */
  void setCursorCaptured(bool captured);

 private:
  GLFWwindow* window;
  int width;
  int height;
  std::string title;

  /**
   * @brief Inicializa GLFW y crea la ventana
   */
  void init();

  /**
   * @brief Callback para manejar cambios de tamaño
   */
  static void framebufferSizeCallback(GLFWwindow* window, int width,
                                      int height);
};

}  // namespace Engine

#endif  // WINDOW_HPP
