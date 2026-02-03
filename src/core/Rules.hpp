/**
 * @file Rules.hpp
 * @brief Definición de reglas de autómatas celulares
 *
 * Diferentes conjuntos de reglas para simulación.
 */

#ifndef RULES_HPP
#define RULES_HPP

#include "Grid2D.hpp"
#include <string>

namespace Core {

/**
 * @enum RuleType
 * @brief Tipos de reglas disponibles
 */
enum class RuleType {
  CONWAY,     // Game of Life clásico
  SEEDS,      // Más caótico
  HIGHLIFE,   // Game of Life + replicadores
  DAY_NIGHT,  // Reglas simétricas
  COUNT
};

/**
 * @class Rules
 * @brief Conjunto de reglas para evolución celular
 */
class Rules {
 public:
  /**
   * @brief Obtiene el nombre de un tipo de regla
   * @param type Tipo de regla
   * @return Nombre legible
   */
  static std::string getName(RuleType type);

  /**
   * @brief Aplica reglas según el tipo
   * @param type Tipo de regla
   * @param currentState Estado actual de la celda
   * @param neighbors Número de vecinos vivos
   * @return Nuevo estado
   */
  static CellState apply(RuleType type, CellState currentState, int neighbors);

 private:
  // Reglas específicas
  static CellState conway(CellState current, int neighbors);
  static CellState seeds(CellState current, int neighbors);
  static CellState highLife(CellState current, int neighbors);
  static CellState dayNight(CellState current, int neighbors);
};

}  // namespace Core

#endif  // RULES_HPP
