/**
 * @file Simulator.hpp
 * @brief Motor de simulación para autómatas celulares
 *
 * Aplica reglas de evolución al grid.
 */

#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "Grid2D.hpp"
#include "Rules.hpp"

namespace Core {

/**
 * @class Simulator
 * @brief Aplica reglas de evolución al grid
 */
class Simulator {
   public:
    /**
     * @brief Constructor
     * @param grid Referencia al grid a simular
     */
    Simulator(Grid2D& grid);

    /**
     * @brief Ejecuta un paso de simulación (Game of Life)
     */
    void step();

    /**
     * @brief Establece si la simulación está pausada
     * @param paused true para pausar
     */
    void setPaused(bool paused) {
        this->paused = paused;
    }

    /**
     * @brief Verifica si está pausada
     * @return true si está pausada
     */
    bool isPaused() const {
        return paused;
    }

    /**
     * @brief Alterna pausa/ejecución
     */
    void togglePause() {
        paused = !paused;
    }

    /**
     * @brief Establece la velocidad de simulación
     * @param stepsPerSecond Pasos por segundo
     */
    void setSpeed(float stepsPerSecond) {
        updateInterval = 1.0f / stepsPerSecond;
    }

    /**
     * @brief Establece el tipo de reglas
     * @param type Tipo de reglas a usar
     */
    void setRuleType(RuleType type) {
        currentRule = type;
    }

    /**
     * @brief Obtiene el tipo de reglas actual
     * @return Tipo de reglas
     */
    RuleType getRuleType() const {
        return currentRule;
    }

    /**
     * @brief Cambia a la siguiente regla
     */
    void nextRule();

    /**
     * @brief Obtiene el número de generación actual
     * @return Generación
     */
    int getGeneration() const {
        return generation;
    }

    /**
     * @brief Reinicia el contador de generaciones
     */
    void resetGeneration() {
        generation = 0;
    }

    /**
     * @brief Actualiza el simulador (llama step() según tiempo)
     * @param deltaTime Tiempo desde el último frame
     */
    void update(float deltaTime);

   private:
    Grid2D& grid;
    bool paused;
    float updateInterval;  // Segundos entre updates
    float accumulator;     // Acumulador de tiempo
    RuleType currentRule;  // Regla actual
    int generation;        // Contador de generaciones
};

}  // namespace Core

#endif  // SIMULATOR_HPP
