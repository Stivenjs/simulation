/**
 * @file Application.cpp
 * @brief Implementación de Application
 */

#include <GL/glew.h>
#include "Application.hpp"
#include "FileFinder.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace Core {

static const char* const MODELS_DIRECTORY = "assets/models";

Application::Application(int width, int height, const std::string& title) :
    width(width), height(height), title(title), lastFrame(0.0f), deltaTime(0.0f)
{
}

void Application::init()
{
    std::cout << "Initializing 3D Simulation Engine..." << std::endl;

    // Crear ventana
    window = std::make_unique<Engine::Window>(width, height, title);

    // Cargar shaders
    std::cout << "Loading shaders..." << std::endl;
    shader = Renderer::Shader::fromFiles("shaders/basic.vert", "shaders/basic.frag");

    // Crear cámara
    camera = std::make_unique<Renderer::Camera>(glm::vec3(0.0f, 2.0f, 5.0f));

    // Crear UI (después de crear ventana)
    ui = std::make_unique<Renderer::UI>(window->getHandle());

    // Nota: InputManager se crea después del grid y simulator

    // Crear grid de simulación (20x20)
    grid = std::make_unique<Grid2D>(20, 20);
    grid->randomize(0.3f);  // 30% de celdas vivas

    // Crear simulador
    simulator = std::make_unique<Simulator>(*grid);
    simulator->setSpeed(5.0f);  // 5 pasos por segundo

    // Crear estadísticas
    stats = std::make_unique<Stats>();

    // Cargar todos los modelos 3D de la carpeta assets/models/
    std::cout << "Loading models from: " << MODELS_DIRECTORY << std::endl;
    std::vector<std::string> modelExtensions = { ".obj", ".fbx", ".gltf", ".glb", ".dae", ".3ds", ".blend" };
    std::vector<std::string> modelFiles = FileFinder::findFilesInDirectory(MODELS_DIRECTORY, modelExtensions);

    if (modelFiles.empty()) {
        std::cout << "  No se encontraron modelos en " << MODELS_DIRECTORY << std::endl;
    } else {
        std::cout << "  Encontrados " << modelFiles.size() << " archivos de modelo" << std::endl;
        for (const auto& modelPath : modelFiles) {
            try {
                auto model = std::make_unique<Renderer::Model>(modelPath);
                if (model->getMeshCount() > 0) {
                    loadedModels.push_back(std::move(model));
                    std::cout << "    ✓ Cargado: " << modelPath << " (" << loadedModels.back()->getMeshCount()
                              << " meshes)" << std::endl;
                } else {
                    std::cerr << "    ✗ Error: " << modelPath << " no tiene meshes válidos" << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "    ✗ Error cargando " << modelPath << ": " << e.what() << std::endl;
            }
        }
        std::cout << "  Total modelos cargados: " << loadedModels.size() << std::endl;
    }

    // Crear input manager (después de grid y simulator)
    inputManager = std::make_unique<InputManager>(window->getHandle(), *camera, *grid, *simulator);

    // Configurar OpenGL
    glEnable(GL_DEPTH_TEST);

    // Configurar iluminación

    // Luz direccional (simula el sol)
    Renderer::DirectionalLight dirLight;
    dirLight.direction = glm::vec3(-0.3f, -1.0f, -0.5f);
    dirLight.ambient = glm::vec3(0.12f);
    dirLight.diffuse = glm::vec3(0.7f);
    dirLight.specular = glm::vec3(0.9f);
    lightManager.setDirectionalLight(dirLight);

    // Luz puntual 1 (encima del grid)
    Renderer::PointLight pointLight1;
    pointLight1.position = glm::vec3(0.0f, 5.0f, 0.0f);
    pointLight1.ambient = glm::vec3(0.05f);
    pointLight1.diffuse = glm::vec3(0.6f, 0.6f, 0.8f);
    pointLight1.specular = glm::vec3(1.0f);
    pointLight1.constant = 1.0f;
    pointLight1.linear = 0.045f;
    pointLight1.quadratic = 0.0075f;
    lightManager.addPointLight(pointLight1);

    // Material por defecto para los cubos
    material.ambient = glm::vec3(1.0f);
    material.diffuse = glm::vec3(1.0f);
    material.specular = glm::vec3(0.5f);
    material.shininess = 32.0f;

    std::cout << "\nGrid initialized: " << grid->getWidth() << "x" << grid->getHeight() << std::endl;
    std::cout << "Simulation: Game of Life (Conway)" << std::endl;
    std::cout << "\nControls:" << std::endl;
    std::cout << "  SPACE: Pause/Resume simulation" << std::endl;
    std::cout << "  R: Randomize grid" << std::endl;
    std::cout << "  C: Clear grid" << std::endl;
    std::cout << "  N: Next rule set" << std::endl;
    std::cout << "  Mouse Left + Drag: Rotate camera" << std::endl;
    std::cout << "  Mouse Scroll: Zoom in/out" << std::endl;
    std::cout << "  WASD: Pan camera | Q/E: Up/Down" << std::endl;
    std::cout << "  ESC: Exit\n" << std::endl;
}

void Application::run()
{
    while (!window->shouldClose()) {
        calculateDeltaTime();
        update();
        render();
        window->swapBuffers();
        window->pollEvents();
    }

    std::cout << "Shutting down..." << std::endl;
}

void Application::update()
{
    inputManager->processKeyboard(deltaTime);
    simulator->update(deltaTime);
    stats->update(*grid, deltaTime);

    // Mostrar stats cada segundo
    static float statsTimer = 0.0f;
    statsTimer += deltaTime;
    if (statsTimer >= 1.0f) {
        std::cout << "\r" << Rules::getName(simulator->getRuleType()) << " | Gen: " << simulator->getGeneration()
                  << " | " << stats->toString() << "          " << std::flush;
        statsTimer = 0.0f;
    }
}

void Application::render()
{
    // Limpiar pantalla
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();

    // Matriz de vista desde la cámara
    glm::mat4 view = camera->getViewMatrix();

    // Matriz de proyección (usar tamaño actual de la ventana)
    int currentWidth = window->getWidth();
    int currentHeight = window->getHeight();
    glm::mat4 projection
        = glm::perspective(glm::radians(45.0f), (float)currentWidth / (float)currentHeight, 0.1f, 100.0f);

    shader->setMat4("view", glm::value_ptr(view));
    shader->setMat4("projection", glm::value_ptr(projection));

    // Enviar posición de la cámara (necesaria para specular)
    glm::vec3 camPos = camera->getPosition();
    shader->setVec3("viewPos", camPos.x, camPos.y, camPos.z);

    // Enviar datos de iluminación y material
    lightManager.apply(*shader);
    material.apply(*shader);

    // Renderizar todos los modelos 3D cargados
    for (size_t i = 0; i < loadedModels.size(); ++i) {
        glm::mat4 modelMat = glm::mat4(1.0f);
        // Distribuir modelos en una línea horizontal
        float offsetX = (static_cast<float>(i) - static_cast<float>(loadedModels.size() - 1) / 2.0f) * 3.0f;
        modelMat = glm::translate(modelMat, glm::vec3(offsetX, 0.0f, 0.0f));
        modelMat = glm::scale(modelMat, glm::vec3(0.5f));  // Escala ajustable
        shader->setMat4("model", glm::value_ptr(modelMat));
        shader->setVec3("cellColor", 1.0f, 1.0f, 1.0f);
        loadedModels[i]->draw(*shader);
    }

    shader->unuse();

    // Renderizar UI (debe ser después de la escena 3D)
    ui->newFrame();
    ui->renderStatsPanel(*simulator, *stats, *grid);
    ui->renderVideoSettingsPanel(*window);
    ui->render();
}

void Application::calculateDeltaTime()
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

}  // namespace Core