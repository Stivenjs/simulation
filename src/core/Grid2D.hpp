/**
 * @file Grid2D.hpp
 * @brief Grilla 2D para simulación de autómatas celulares
 *
 * Representa el estado del "universo" discreto en 2D.
 */

#ifndef GRID2D_HPP
#define GRID2D_HPP

#include <vector>
#include <cstdint>

namespace Core {

/**
 * @enum CellState
 * @brief Estados posibles de una celda
 */
enum class CellState : uint8_t {
  DEAD = 0,   // Vacío/muerto
  ALIVE = 1,  // Vivo/activo
  COUNT       // Número de estados
};

/**
 * @class Grid2D
 * @brief Grilla 2D con estados discretos por celda
 */
class Grid2D {
 public:
  /**
   * @brief Constructor
   * @param width Ancho de la grilla
   * @param height Alto de la grilla
   */
  Grid2D(int width, int height);

  /**
   * @brief Obtiene el estado de una celda
   * @param x Coordenada X
   * @param y Coordenada Y
   * @return Estado de la celda
   */
  CellState getCell(int x, int y) const;

  /**
   * @brief Establece el estado de una celda
   * @param x Coordenada X
   * @param y Coordenada Y
   * @param state Nuevo estado
   */
  void setCell(int x, int y, CellState state);

  /**
   * @brief Obtiene el ancho de la grilla
   * @return Ancho
   */
  int getWidth() const {
    return width;
  }

  /**
   * @brief Obtiene el alto de la grilla
   * @return Alto
   */
  int getHeight() const {
    return height;
  }

  /**
   * @brief Limpia la grilla (todas las celdas a DEAD)
   */
  void clear();

  /**
   * @brief Llena la grilla aleatoriamente
   * @param probability Probabilidad de celda viva (0.0 a 1.0)
   */
  void randomize(float probability = 0.3f);

  /**
   * @brief Cuenta vecinos vivos de una celda (vecindario de Moore - 8 vecinos)
   * @param x Coordenada X
   * @param y Coordenada Y
   * @return Número de vecinos vivos
   */
  int countAliveNeighbors(int x, int y) const;

 private:
  int width;
  int height;
  std::vector<CellState> cells;

  /**
   * @brief Convierte coordenadas 2D a índice 1D
   * @param x Coordenada X
   * @param y Coordenada Y
   * @return Índice en el vector
   */
  int getIndex(int x, int y) const;

  /**
   * @brief Verifica si las coordenadas están dentro de los límites
   * @param x Coordenada X
   * @param y Coordenada Y
   * @return true si están dentro
   */
  bool isValid(int x, int y) const;
};

}  // namespace Core

#endif  // GRID2D_HPP
