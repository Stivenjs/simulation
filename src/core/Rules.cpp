/**
 * @file Rules.cpp
 * @brief Implementación de reglas de autómatas celulares
 */

#include "Rules.hpp"

namespace Core {

std::string Rules::getName(RuleType type) {
    switch (type) {
        case RuleType::CONWAY:
            return "Conway's Game of Life";
        case RuleType::SEEDS:
            return "Seeds";
        case RuleType::HIGHLIFE:
            return "HighLife";
        case RuleType::DAY_NIGHT:
            return "Day & Night";
        default:
            return "Unknown";
    }
}

CellState Rules::apply(RuleType type, CellState currentState, int neighbors) {
    switch (type) {
        case RuleType::CONWAY:
            return conway(currentState, neighbors);
        case RuleType::SEEDS:
            return seeds(currentState, neighbors);
        case RuleType::HIGHLIFE:
            return highLife(currentState, neighbors);
        case RuleType::DAY_NIGHT:
            return dayNight(currentState, neighbors);
        default:
            return currentState;
    }
}

CellState Rules::conway(CellState current, int neighbors) {
    // Conway's Game of Life (B3/S23)
    if (current == CellState::ALIVE) {
        if (neighbors < 2 || neighbors > 3)
            return CellState::DEAD;
        return CellState::ALIVE;
    } else {
        if (neighbors == 3)
            return CellState::ALIVE;
        return CellState::DEAD;
    }
}

CellState Rules::seeds(CellState current, int neighbors) {
    // Seeds (B2/S)
    // Células vivas siempre mueren, solo nacen con 2 vecinos
    if (current == CellState::DEAD && neighbors == 2) {
        return CellState::ALIVE;
    }
    return CellState::DEAD;
}

CellState Rules::highLife(CellState current, int neighbors) {
    // HighLife (B36/S23)
    // Como Conway pero también nace con 6 vecinos
    if (current == CellState::ALIVE) {
        if (neighbors < 2 || neighbors > 3)
            return CellState::DEAD;
        return CellState::ALIVE;
    } else {
        if (neighbors == 3 || neighbors == 6)
            return CellState::ALIVE;
        return CellState::DEAD;
    }
}

CellState Rules::dayNight(CellState current, int neighbors) {
    // Day & Night (B3678/S34678)
    // Reglas simétricas: Birth y Survival son casi iguales
    if (current == CellState::ALIVE) {
        // Sobrevive con 3,4,6,7,8 vecinos
        if (neighbors == 3 || neighbors == 4 || neighbors >= 6)
            return CellState::ALIVE;
        return CellState::DEAD;
    } else {
        // Nace con 3,6,7,8 vecinos
        if (neighbors == 3 || neighbors >= 6)
            return CellState::ALIVE;
        return CellState::DEAD;
    }
}

}  // namespace Core
