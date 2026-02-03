/**
 * @file Grid2D.cpp
 * @brief Implementación de Grid2D
 */

#include "Grid2D.hpp"
#include <random>
#include <algorithm>

namespace Core {

Grid2D::Grid2D(int width, int height)
    : width(width), height(height), cells(width * height, CellState::DEAD) {}

CellState Grid2D::getCell(int x, int y) const {
    if (!isValid(x, y))
        return CellState::DEAD;
    return cells[getIndex(x, y)];
}

void Grid2D::setCell(int x, int y, CellState state) {
    if (isValid(x, y)) {
        cells[getIndex(x, y)] = state;
    }
}

void Grid2D::clear() {
    std::fill(cells.begin(), cells.end(), CellState::DEAD);
}

void Grid2D::randomize(float probability) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            CellState state =
                (dis(gen) < probability) ? CellState::ALIVE : CellState::DEAD;
            setCell(x, y, state);
        }
    }
}

int Grid2D::countAliveNeighbors(int x, int y) const {
    int count = 0;

    // Vecindario de Moore (8 vecinos)
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            // Saltar la celda central
            if (dx == 0 && dy == 0)
                continue;

            int nx = x + dx;
            int ny = y + dy;

            if (isValid(nx, ny) && getCell(nx, ny) == CellState::ALIVE) {
                count++;
            }
        }
    }

    return count;
}

int Grid2D::getIndex(int x, int y) const {
    return y * width + x;
}

bool Grid2D::isValid(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

void Grid2D::getCellColor(int x, int y, float& r, float& g, float& b) const {
    CellState state = getCell(x, y);

    if (state == CellState::DEAD) {
        // Células muertas muy oscuras
        r = 0.1f;
        g = 0.1f;
        b = 0.15f;
        return;
    }

    // Células vivas: color según número de vecinos
    int neighbors = countAliveNeighbors(x, y);
    float intensity = 0.3f + (neighbors / 8.0f) * 0.7f;  // 0.3 a 1.0

    // Gradiente: azul (pocos) -> cyan -> verde -> amarillo (muchos)
    if (neighbors <= 2) {
        r = 0.0f;
        g = neighbors / 2.0f * 0.5f;
        b = intensity;
    } else if (neighbors <= 4) {
        r = 0.0f;
        g = intensity;
        b = (4.0f - neighbors) / 2.0f * intensity;
    } else if (neighbors <= 6) {
        r = (neighbors - 4.0f) / 2.0f * intensity;
        g = intensity;
        b = 0.0f;
    } else {
        r = intensity;
        g = (8.0f - neighbors) / 2.0f * 0.5f;
        b = 0.0f;
    }
}

}  // namespace Core
