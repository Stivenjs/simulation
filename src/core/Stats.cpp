/**
 * @file Stats.cpp
 * @brief Implementación de Stats
 */

#include "Stats.hpp"
#include <sstream>
#include <iomanip>

namespace Core {

Stats::Stats()
    : population(0), fps(0.0f), fpsAccumulator(0.0f), frameCount(0) {}

void Stats::update(const Grid2D& grid, float deltaTime) {
    // Calcular población
    population = 0;
    for (int y = 0; y < grid.getHeight(); ++y) {
        for (int x = 0; x < grid.getWidth(); ++x) {
            if (grid.getCell(x, y) == CellState::ALIVE) {
                population++;
            }
        }
    }

    // Calcular FPS
    frameCount++;
    fpsAccumulator += deltaTime;

    if (fpsAccumulator >= 1.0f) {
        fps = frameCount / fpsAccumulator;
        frameCount = 0;
        fpsAccumulator = 0.0f;
    }
}

std::string Stats::toString() const {
    std::stringstream ss;
    ss << "Population: " << std::setw(4) << population
       << " | FPS: " << std::fixed << std::setprecision(1) << fps;
    return ss.str();
}

}  // namespace Core
