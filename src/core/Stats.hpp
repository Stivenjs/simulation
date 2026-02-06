/**
 * @file Stats.hpp
 * @brief Estadísticas de la simulación
 *
 * Rastrea métricas de rendimiento y simulación.
 */

#ifndef STATS_HPP
#define STATS_HPP

#include "Grid2D.hpp"
#include <string>

namespace Core {

/**
 * @class Stats
 * @brief Calcula y mantiene estadísticas de la simulación
 */
class Stats {
public:
    /**
     * @brief Constructor
     */
    Stats();

    /**
     * @brief Actualiza estadísticas
     * @param grid Grid actual
     * @param deltaTime Tiempo del frame
     */
    void update(const Grid2D& grid, float deltaTime);

    /**
     * @brief Obtiene población actual
     * @return Número de células vivas
     */
    int getPopulation() const { return population; }

    /**
     * @brief Obtiene FPS actual
     * @return Frames por segundo
     */
    float getFPS() const { return fps; }

    /**
     * @brief Obtiene estadísticas como string
     * @return String formateado
     */
    std::string toString() const;

private:
    int population;
    float fps;
    float fpsAccumulator;
    int frameCount;
};

}  // namespace Core

#endif  // STATS_HPP
