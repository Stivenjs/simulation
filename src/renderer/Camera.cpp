/**
 * @file Camera.cpp
 * @brief Implementación de la cámara orbital
 */

#include "Camera.hpp"
#include <algorithm>

namespace Renderer {

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up)
    : target(target),
      up(up),
      distance(glm::length(position - target)),
      yaw(-90.0f),
      pitch(0.0f),
      mouseSensitivity(0.15f),
      scrollSensitivity(0.5f),
      panSpeed(2.5f),
      minDistance(1.0f),
      maxDistance(20.0f),
      minPitch(-89.0f),
      maxPitch(89.0f),
      position(position) {
  // Calcular yaw y pitch iniciales desde la posición
  glm::vec3 direction = glm::normalize(position - target);
  yaw = glm::degrees(atan2(direction.z, direction.x));
  pitch = glm::degrees(asin(direction.y));

  updateCameraPosition();
}

glm::mat4 Camera::getViewMatrix() const {
  return glm::lookAt(position, target, up);
}

void Camera::processMouseMovement(float xoffset, float yoffset) {
  xoffset *= mouseSensitivity;
  yoffset *= mouseSensitivity;

  yaw += xoffset;
  pitch += yoffset;

  // Limitar pitch para evitar gimbal lock
  pitch = std::clamp(pitch, minPitch, maxPitch);

  updateCameraPosition();
}

void Camera::processMouseScroll(float yoffset) {
  distance -= yoffset * scrollSensitivity;
  distance = std::clamp(distance, minDistance, maxDistance);

  updateCameraPosition();
}

void Camera::pan(glm::vec3 direction, float deltaTime) {
  // Calcular vectores de la cámara
  glm::vec3 forward = glm::normalize(target - position);
  glm::vec3 right = glm::normalize(glm::cross(forward, up));
  glm::vec3 cameraUp = glm::normalize(glm::cross(right, forward));

  // Aplicar movimiento
  glm::vec3 movement = glm::vec3(0.0f);
  movement += right * direction.x;
  movement += cameraUp * direction.y;
  movement += forward * direction.z;

  float velocity = panSpeed * deltaTime;
  target += movement * velocity;

  updateCameraPosition();
}

void Camera::updateCameraPosition() {
  // Convertir coordenadas esféricas a cartesianas
  float yawRad = glm::radians(yaw);
  float pitchRad = glm::radians(pitch);

  position.x = target.x + distance * cos(pitchRad) * cos(yawRad);
  position.y = target.y + distance * sin(pitchRad);
  position.z = target.z + distance * cos(pitchRad) * sin(yawRad);
}

}  // namespace Renderer
