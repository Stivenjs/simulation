/**
 * @file main.cpp
 * @brief Motor de simulación de universo discreto
 * 
 * Programa principal que demuestra el funcionamiento básico del motor.
 */

#include <iostream>
#include <vector>

/**
 * @brief Punto de entrada principal del programa
 * 
 * Inicializa el sistema y ejecuta una demostración básica
 * de las capacidades del motor de simulación.
 * 
 * @return int Código de retorno (0 = éxito)
 */
int main() {
    // Mensaje de bienvenida
    std::cout << "Hello World from C++20!" << std::endl;
    std::cout << "Simulation Engine - Universe Discrete Motor" << std::endl;
    
    // Demostración de características de C++20
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    
    std::cout << "\nNumbers: ";
    for (const auto& num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    std::cout << "\nSystem initialized successfully." << std::endl;
    
    return 0;
}
