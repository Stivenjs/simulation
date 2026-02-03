/**
 * @file UI.hpp
 * @brief Sistema de UI con Dear ImGui
 *
 * Maneja la interfaz de usuario para debug y control.
 */

#ifndef UI_HPP
#define UI_HPP

#include <GLFW/glfw3.h>
#include "core/Simulator.hpp"
#include "core/Stats.hpp"
#include "core/Grid2D.hpp"

namespace Renderer {

/**
 * @class UI
 * @brief Interfaz de usuario con ImGui
 */
class UI {
 public:
  /**
   * @brief Constructor
   * @param window Puntero a ventana GLFW
   * @param glsl_version Versión de GLSL a usar
   */
  UI(GLFWwindow* window, const char* glsl_version = "#version 330");

  /**
   * @brief Destructor - Limpia recursos de ImGui
   */
  ~UI();

  // Prevenir copia
  UI(const UI&) = delete;
  UI& operator=(const UI&) = delete;

  /**
   * @brief Inicia un nuevo frame de UI
   */
  void newFrame();

  /**
   * @brief Renderiza la UI
   */
  void render();

  /**
   * @brief Renderiza panel de estadísticas
   * @param simulator Referencia al simulador
   * @param stats Referencia a estadísticas
   * @param grid Referencia al grid
   */
  void renderStatsPanel(Core::Simulator& simulator, Core::Stats& stats,
                        Core::Grid2D& grid);

 private:
  bool showStatsWindow;
  bool showControlsWindow;
};

}  // namespace Renderer

#endif  // UI_HPP
