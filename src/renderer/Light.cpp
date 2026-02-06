/**
 * @file Light.cpp
 * @brief Implementación del sistema de iluminación
 */

#include "Light.hpp"
#include "Shader.hpp"

namespace Renderer {

void DirectionalLight::apply(const Shader& shader) const
{
    shader.setVec3("dirLight.direction", direction.x, direction.y, direction.z);
    shader.setVec3("dirLight.ambient", ambient.x, ambient.y, ambient.z);
    shader.setVec3("dirLight.diffuse", diffuse.x, diffuse.y, diffuse.z);
    shader.setVec3("dirLight.specular", specular.x, specular.y, specular.z);
}

void PointLight::apply(const Shader& shader, int index) const
{
    std::string prefix = "pointLights[" + std::to_string(index) + "]";

    shader.setVec3(prefix + ".position", position.x, position.y, position.z);
    shader.setVec3(prefix + ".ambient", ambient.x, ambient.y, ambient.z);
    shader.setVec3(prefix + ".diffuse", diffuse.x, diffuse.y, diffuse.z);
    shader.setVec3(prefix + ".specular", specular.x, specular.y, specular.z);
    shader.setFloat(prefix + ".constant", constant);
    shader.setFloat(prefix + ".linear", linear);
    shader.setFloat(prefix + ".quadratic", quadratic);
}

void Material::apply(const Shader& shader) const
{
    shader.setVec3("material.ambient", ambient.x, ambient.y, ambient.z);
    shader.setVec3("material.diffuse", diffuse.x, diffuse.y, diffuse.z);
    shader.setVec3("material.specular", specular.x, specular.y, specular.z);
    shader.setFloat("material.shininess", shininess);
}

bool LightManager::addPointLight(const PointLight& light)
{
    if (static_cast<int>(pointLights.size()) >= MAX_POINT_LIGHTS) {
        return false;
    }
    pointLights.push_back(light);
    return true;
}

void LightManager::apply(const Shader& shader) const
{
    // Enviar luz direccional
    dirLight.apply(shader);

    // Enviar luces puntuales
    shader.setInt("numPointLights", static_cast<int>(pointLights.size()));
    for (int i = 0; i < static_cast<int>(pointLights.size()); i++) {
        pointLights[i].apply(shader, i);
    }
}

}  // namespace Renderer
