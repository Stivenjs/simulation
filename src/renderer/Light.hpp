/**
 * @file Light.hpp
 * @brief Sistema de iluminación para el motor 3D
 *
 * Soporta luces direccionales y puntuales con
 * modelo de iluminación Phong.
 */

#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glm/glm.hpp>
#include <vector>

namespace Renderer {

class Shader;

/**
 * @struct DirectionalLight
 * @brief Luz direccional (tipo sol) - ilumina todo en una dirección
 */
struct DirectionalLight {
    glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    glm::vec3 ambient = glm::vec3(0.15f);
    glm::vec3 diffuse = glm::vec3(0.8f);
    glm::vec3 specular = glm::vec3(1.0f);

    /**
     * @brief Envía los datos de la luz al shader
     * @param shader Referencia al shader
     */
    void apply(const Shader& shader) const;
};

/**
 * @struct PointLight
 * @brief Luz puntual - emite luz desde un punto en todas las direcciones
 */
struct PointLight {
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 ambient = glm::vec3(0.05f);
    glm::vec3 diffuse = glm::vec3(0.8f);
    glm::vec3 specular = glm::vec3(1.0f);

    // Atenuación (disminución con la distancia)
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;

    /**
     * @brief Envía los datos de la luz al shader
     * @param shader Referencia al shader
     * @param index Índice de la luz en el array del shader
     */
    void apply(const Shader& shader, int index) const;
};

/**
 * @struct Material
 * @brief Propiedades de material para iluminación Phong
 */
struct Material {
    glm::vec3 ambient = glm::vec3(1.0f);
    glm::vec3 diffuse = glm::vec3(1.0f);
    glm::vec3 specular = glm::vec3(0.5f);
    float shininess = 32.0f;

    /**
     * @brief Envía los datos del material al shader
     * @param shader Referencia al shader
     */
    void apply(const Shader& shader) const;
};

/**
 * @class LightManager
 * @brief Gestiona todas las luces de la escena
 */
class LightManager {
public:
    static constexpr int MAX_POINT_LIGHTS = 4;

    LightManager() = default;

    /**
     * @brief Establece la luz direccional
     * @param light Luz direccional
     */
    void setDirectionalLight(const DirectionalLight& light) { dirLight = light; }

    /**
     * @brief Obtiene referencia a la luz direccional
     * @return Referencia a la luz direccional
     */
    DirectionalLight& getDirectionalLight() { return dirLight; }

    /**
     * @brief Agrega una luz puntual
     * @param light Luz puntual a agregar
     * @return true si se agregó correctamente
     */
    bool addPointLight(const PointLight& light);

    /**
     * @brief Obtiene referencia a una luz puntual
     * @param index Índice de la luz
     * @return Referencia a la luz puntual
     */
    PointLight& getPointLight(int index) { return pointLights[index]; }

    /**
     * @brief Obtiene el número de luces puntuales activas
     * @return Número de luces
     */
    int getPointLightCount() const { return static_cast<int>(pointLights.size()); }

    /**
     * @brief Envía todas las luces al shader
     * @param shader Referencia al shader
     */
    void apply(const Shader& shader) const;

private:
    DirectionalLight dirLight;
    std::vector<PointLight> pointLights;
};

}  // namespace Renderer

#endif  // LIGHT_HPP
