/**
 * @file Application.hpp
 * @brief Clase principal de la aplicación
 *
 * Similar a una clase App en frameworks modernos
 */

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "renderer/Shader.hpp"
#include "engine/Window.hpp"
#include "renderer/Camera.hpp"
#include "renderer/Light.hpp"
#include "renderer/Mesh.hpp"
#include "renderer/UI.hpp"
#include "model/Model.hpp"
#include "InputManager.hpp"
#include "Grid2D.hpp"
#include "Simulator.hpp"
#include "Stats.hpp"
#include <memory>

namespace Core {

/**
 * @class Application
 * @brief Coordina el motor: ventana, renderizado, input, simulación
 */
class Application {
public:
    /**
     * @brief Constructor
     * @param width Ancho de la ventana
     * @param height Alto de la ventana
     * @param title Título de la ventana
     */
    Application(int width, int height, const std::string& title);

    /**
     * @brief Destructor
     */
    ~Application() = default;

    /**
     * @brief Inicializa la aplicación
     */
    void init();

    /**
     * @brief Ejecuta el loop principal
     */
    void run();

private:
    // Componentes principales
    std::unique_ptr<Engine::Window> window;
    std::unique_ptr<Renderer::Shader> shader;
    std::unique_ptr<Renderer::Camera> camera;
    std::unique_ptr<Renderer::UI> ui;
    std::unique_ptr<InputManager> inputManager;

    // Simulación
    std::unique_ptr<Grid2D> grid;
    std::unique_ptr<Simulator> simulator;
    std::unique_ptr<Stats> stats;
    std::unique_ptr<Renderer::Mesh> cubeMesh;
    std::vector<std::unique_ptr<Renderer::Model>> loadedModels;

    // Iluminación
    Renderer::LightManager lightManager;
    Renderer::Material material;

    // Configuración
    int width, height;
    std::string title;

    // Tiempo
    float lastFrame;
    float deltaTime;

    /**
     * @brief Actualiza la lógica del juego/simulación
     */
    void update();

    /**
     * @brief Renderiza la escena
     */
    void render();

    /**
     * @brief Calcula delta time
     */
    void calculateDeltaTime();
};

}  // namespace Core

#endif  // APPLICATION_HPP