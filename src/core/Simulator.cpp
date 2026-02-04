/**
 * @file Simulator.cpp
 * @brief Implementación del motor de simulación
 */

#include "Simulator.hpp"

namespace Core {

Simulator::Simulator(Grid2D& grid) :
    grid(grid), paused(true), updateInterval(0.1f), accumulator(0.0f), currentRule(RuleType::CONWAY), generation(0)
{
}

void Simulator::step()
{
    int width = grid.getWidth();
    int height = grid.getHeight();

    // Crear buffer temporal para el nuevo estado
    std::vector<CellState> newStates(width * height);

    // Calcular nuevo estado para cada celda
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int neighbors = grid.countAliveNeighbors(x, y);
            CellState currentState = grid.getCell(x, y);
            newStates[y * width + x] = Rules::apply(currentRule, currentState, neighbors);
        }
    }

    // Aplicar nuevo estado al grid
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            grid.setCell(x, y, newStates[y * width + x]);
        }
    }
}

void Simulator::nextRule()
{
    int nextRuleIndex = (static_cast<int>(currentRule) + 1) % static_cast<int>(RuleType::COUNT);
    currentRule = static_cast<RuleType>(nextRuleIndex);
}

void Simulator::update(float deltaTime)
{
    if (paused)
        return;

    accumulator += deltaTime;

    // Ejecutar steps según el tiempo acumulado
    while (accumulator >= updateInterval) {
        step();
        generation++;
        accumulator -= updateInterval;
    }
}

}  // namespace Core
