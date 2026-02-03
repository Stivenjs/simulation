/**
 * @file InputManager.hpp
 * @brief Gestión centralizada de input (mouse y teclado)
 *
 */

#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include <GLFW/glfw3.h>
#include "Grid2D.hpp"
#include "Simulator.hpp"
#include "renderer/Camera.hpp"

namespace Core {

/**
 * @class InputManager
 * @brief Maneja todos los eventos de input
 */
class InputManager {
   public:
    /**
     * @brief Constructor
     * @param window Puntero a la ventana GLFW
     * @param camera Referencia a la cámara
     * @param grid Referencia al grid de simulación
     * @param simulator Referencia al simulador
     */
    InputManager(GLFWwindow* window, Renderer::Camera& camera, Grid2D& grid,
                 Simulator& simulator);

    /**
     * @brief Procesa input del teclado
     * @param deltaTime Tiempo desde el último frame
     */
    void processKeyboard(float deltaTime);

    /**
     * @brief Verifica si se debe cerrar la aplicación
     * @return true si se debe cerrar
     */
    bool shouldClose() const;

   private:
    GLFWwindow* window;
    Renderer::Camera& camera;
    Grid2D& grid;
    Simulator& simulator;

    // Estado del mouse
    float lastX, lastY;
    bool firstMouse;
    bool mousePressed;

    // Callbacks estáticos
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action,
                                    int mods);
    static void scrollCallback(GLFWwindow* window, double xoffset,
                               double yoffset);

    // Instancia global para callbacks (no ideal, pero necesario para GLFW)
    static InputManager* instance;
};

}  // namespace Core

#endif  // INPUT_MANAGER_HPP
