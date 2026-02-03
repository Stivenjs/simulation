/**
 * @file Simulator.hpp
 * @brief Motor de simulación para autómatas celulares
 *
 * Aplica reglas de evolución al grid.
 */

#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "Grid2D.hpp"

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
   * @brief Actualiza el simulador (llama step() según tiempo)
   * @param deltaTime Tiempo desde el último frame
   */
  void update(float deltaTime);

 private:
  Grid2D& grid;
  bool paused;
  float updateInterval;  // Segundos entre updates
  float accumulator;     // Acumulador de tiempo

  /**
   * @brief Aplica reglas de Conway's Game of Life
   * @param x Coordenada X
   * @param y Coordenada Y
   * @param neighbors Número de vecinos vivos
   * @return Nuevo estado de la celda
   */
  CellState applyRules(int x, int y, int neighbors) const;
};

}  // namespace Core

#endif  // SIMULATOR_HPP
