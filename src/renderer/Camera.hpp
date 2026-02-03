/**
 * @file Camera.hpp
 * @brief Cámara orbital para navegación 3D
 *
 * Permite rotar alrededor de un punto, hacer zoom y pan.
 */

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Renderer {

/**
 * @class Camera
 * @brief Cámara orbital con controles de mouse y teclado
 */
class Camera {
 public:
  /**
   * @brief Constructor
   * @param position Posición inicial de la cámara
   * @param target Punto al que mira la cámara
   * @param up Vector up (generalmente (0,1,0))
   */
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f),
         glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

  /**
   * @brief Obtiene la matriz de vista
   * @return Matriz de vista
   */
  glm::mat4 getViewMatrix() const;

  /**
   * @brief Procesa movimiento del mouse para rotación orbital
   * @param xoffset Desplazamiento en X
   * @param yoffset Desplazamiento en Y
   */
  void processMouseMovement(float xoffset, float yoffset);

  /**
   * @brief Procesa scroll del mouse para zoom
   * @param yoffset Cantidad de scroll
   */
  void processMouseScroll(float yoffset);

  /**
   * @brief Mueve la cámara (pan)
   * @param direction Dirección de movimiento
   * @param deltaTime Tiempo desde el último frame
   */
  void pan(glm::vec3 direction, float deltaTime);

  /**
   * @brief Obtiene la distancia al objetivo
   * @return Distancia
   */
  float getDistance() const {
    return distance;
  }

  /**
   * @brief Obtiene el objetivo de la cámara
   * @return Punto objetivo
   */
  glm::vec3 getTarget() const {
    return target;
  }

 private:
  // Parámetros de la cámara orbital
  glm::vec3 target;  // Punto al que mira
  glm::vec3 up;      // Vector up
  float distance;    // Distancia al objetivo
  float yaw;         // Rotación horizontal (grados)
  float pitch;       // Rotación vertical (grados)

  // Sensibilidad de controles
  float mouseSensitivity;
  float scrollSensitivity;
  float panSpeed;

  // Límites
  float minDistance;
  float maxDistance;
  float minPitch;
  float maxPitch;

  /**
   * @brief Actualiza la posición de la cámara basado en yaw, pitch y distance
   */
  void updateCameraPosition();

  /**
   * @brief Posición calculada de la cámara
   */
  glm::vec3 position;
};

}  // namespace Renderer

#endif  // CAMERA_HPP
