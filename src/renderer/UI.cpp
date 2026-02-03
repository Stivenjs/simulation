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
    : showStatsWindow(true),
      showControlsWindow(true),
      showVideoSettingsWindow(false),
      selectedResolutionIndex(0),
      selectedDisplayModeIndex(0) {
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  // false = no instalar callbacks automáticamente, los manejará InputManager
  ImGui_ImplGlfw_InitForOpenGL(window, false);
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
  ImGui::SetNextWindowSize(ImVec2(300, 250), ImGuiCond_FirstUseEver);

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

  // Botón para abrir configuración de video
  ImGui::Separator();
  if (ImGui::Button("Configuración de Video", ImVec2(-1, 0))) {
    showVideoSettingsWindow = true;
  }

  ImGui::End();
}

void UI::renderVideoSettingsPanel(Engine::Window& window) {
  if (!showVideoSettingsWindow) {
    return;
  }

  ImGui::SetNextWindowPos(ImVec2(330, 10), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(420, 380), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin("Configuración de Video", &showVideoSettingsWindow)) {
    ImGui::End();
    return;
  }

  // Obtener resoluciones disponibles
  static std::vector<Engine::Window::Resolution> resolutions;
  static bool initialized = false;

  if (!initialized) {
    resolutions = window.getAvailableResolutions();
    initialized = true;

    // Encontrar la resolución actual
    int currentWidth = window.getWidth();
    int currentHeight = window.getHeight();
    for (size_t i = 0; i < resolutions.size(); i++) {
      if (resolutions[i].width == currentWidth &&
          resolutions[i].height == currentHeight) {
        selectedResolutionIndex = static_cast<int>(i);
        break;
      }
    }

    // Establecer el modo actual
    selectedDisplayModeIndex = static_cast<int>(window.getDisplayMode());
  }

  ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Resolución");
  ImGui::Separator();
  ImGui::Spacing();

  // Selector de resolución
  std::string currentResLabel = "Sin resoluciones";
  if (!resolutions.empty() && selectedResolutionIndex >= 0 &&
      selectedResolutionIndex < static_cast<int>(resolutions.size())) {
    currentResLabel =
        std::to_string(resolutions[selectedResolutionIndex].width) + " x " +
        std::to_string(resolutions[selectedResolutionIndex].height);
  }

  if (ImGui::BeginCombo("##resolution", currentResLabel.c_str())) {
    for (int i = 0; i < static_cast<int>(resolutions.size()); i++) {
      bool isSelected = (selectedResolutionIndex == i);
      std::string label = std::to_string(resolutions[i].width) + " x " +
                          std::to_string(resolutions[i].height);

      if (ImGui::Selectable(label.c_str(), isSelected)) {
        selectedResolutionIndex = i;
      }

      if (isSelected) {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }

  ImGui::Spacing();
  ImGui::Spacing();
  ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Modo de Pantalla");
  ImGui::Separator();
  ImGui::Spacing();

  // Selector de modo de pantalla
  const char* displayModes[] = {"Ventana", "Pantalla Completa",
                                "Pantalla Completa sin Bordes"};

  ImGui::Combo("##displaymode", &selectedDisplayModeIndex, displayModes,
               IM_ARRAYSIZE(displayModes));

  ImGui::Spacing();
  ImGui::Spacing();
  ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Estado Actual");
  ImGui::Separator();
  ImGui::Spacing();

  // Información actual
  ImGui::Text("Resolución: %d x %d", window.getWidth(), window.getHeight());

  const char* currentMode = "";
  switch (window.getDisplayMode()) {
    case Engine::Window::DisplayMode::Windowed:
      currentMode = "Ventana";
      break;
    case Engine::Window::DisplayMode::Fullscreen:
      currentMode = "Pantalla Completa";
      break;
    case Engine::Window::DisplayMode::BorderlessFullscreen:
      currentMode = "Pantalla Completa sin Bordes";
      break;
  }
  ImGui::Text("Modo: %s", currentMode);

  ImGui::Spacing();
  ImGui::Spacing();
  ImGui::Separator();
  ImGui::Spacing();

  // Botón para aplicar cambios
  if (ImGui::Button("Aplicar Cambios", ImVec2(195, 30))) {
    if (!resolutions.empty() && selectedResolutionIndex >= 0 &&
        selectedResolutionIndex < static_cast<int>(resolutions.size())) {
      Engine::Window::DisplayMode mode =
          static_cast<Engine::Window::DisplayMode>(selectedDisplayModeIndex);

      window.setDisplayMode(resolutions[selectedResolutionIndex].width,
                            resolutions[selectedResolutionIndex].height, mode);
    }
  }

  ImGui::SameLine();

  if (ImGui::Button("Cerrar", ImVec2(195, 30))) {
    showVideoSettingsWindow = false;
  }

  ImGui::End();
}

}  // namespace Renderer
