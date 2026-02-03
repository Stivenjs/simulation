/**
 * @file Simulator.cpp
 * @brief Implementación del motor de simulación
 */

#include "Simulator.hpp"

namespace Core {

Simulator::Simulator(Grid2D& grid)
    : grid(grid), paused(true), updateInterval(0.1f), accumulator(0.0f) {}

void Simulator::step() {
  int width = grid.getWidth();
  int height = grid.getHeight();

  // Crear buffer temporal para el nuevo estado
  std::vector<CellState> newStates(width * height);

  // Calcular nuevo estado para cada celda
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      int neighbors = grid.countAliveNeighbors(x, y);
      newStates[y * width + x] = applyRules(x, y, neighbors);
    }
  }

  // Aplicar nuevo estado al grid
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      grid.setCell(x, y, newStates[y * width + x]);
    }
  }
}

void Simulator::update(float deltaTime) {
  if (paused)
    return;

  accumulator += deltaTime;

  // Ejecutar steps según el tiempo acumulado
  while (accumulator >= updateInterval) {
    step();
    accumulator -= updateInterval;
  }
}

CellState Simulator::applyRules(int x, int y, int neighbors) const {
  CellState currentState = grid.getCell(x, y);

  // Reglas de Conway's Game of Life
  if (currentState == CellState::ALIVE) {
    // Celda viva
    if (neighbors < 2)
      return CellState::DEAD;  // Muere por soledad
    if (neighbors > 3)
      return CellState::DEAD;  // Muere por sobrepoblación
    return CellState::ALIVE;   // Sobrevive con 2-3 vecinos
  } else {
    // Celda muerta
    if (neighbors == 3)
      return CellState::ALIVE;  // Nace con exactamente 3 vecinos
    return CellState::DEAD;
  }
}

}  // namespace Core
