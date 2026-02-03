/**
 * @file main.cpp
 * @brief Motor de Simulación 3D - Punto de entrada
 *
 * Inicializa y ejecuta la aplicación principal.
 */

#include "core/Application.hpp"
#include <iostream>

int main() {
    try {
        Core::Application app(800, 600, "3D Universe Simulation");

        app.init();

        app.run();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
