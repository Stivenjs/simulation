/**
 * @file UI.cpp
 * @brief Implementación de UI con ImGui
 */

#include "UI.hpp"
#include "core/Rules.hpp"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Renderer {

UI::UI(GLFWwindow* window, const char* glsl_version)
    : showStatsWindow(true), showControlsWindow(true) {
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
}

UI::~UI() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void UI::newFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void UI::render() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::renderStatsPanel(Core::Simulator& simulator, Core::Stats& stats,
                          Core::Grid2D& grid) {
  if (!showStatsWindow)
    return;

  ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);

  ImGui::Begin("Simulation Stats", &showStatsWindow);

  // Información de la regla actual
  Core::RuleType ruleType = simulator.getRuleType();
  ImGui::Text("Rule: %s", Core::Rules::getName(ruleType).c_str());
  ImGui::Separator();

  // Estadísticas
  ImGui::Text("Generation: %d", simulator.getGeneration());
  ImGui::Text("Population: %d / %d cells", stats.getPopulation(),
              grid.getWidth() * grid.getHeight());

  float density = (float)stats.getPopulation() /
                  (grid.getWidth() * grid.getHeight()) * 100.0f;
  ImGui::Text("Density: %.1f%%", density);

  ImGui::Separator();
  ImGui::Text("FPS: %.1f", stats.getFPS());

  // Estado de la simulación
  ImGui::Separator();
  const char* status = simulator.isPaused() ? "PAUSED" : "RUNNING";
  ImGui::TextColored(
      simulator.isPaused() ? ImVec4(1, 0.5f, 0, 1) : ImVec4(0, 1, 0, 1),
      "Status: %s", status);

  ImGui::End();
}

}  // namespace Renderer
