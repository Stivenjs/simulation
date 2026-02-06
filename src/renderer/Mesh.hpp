/**
 * @file Mesh.hpp
 * @brief Representación de geometría 3D
 *
 * Similar a THREE.Mesh en Three.js. Soporta vértices con
 * posición, normal, coordenadas UV y color, así como texturas.
 */

#ifndef MESH_HPP
#define MESH_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "Texture.hpp"

namespace Renderer {

// Forward declaration
class Shader;

/**
 * @struct Vertex
 * @brief Estructura de un vértice con posición, normal, UV y color
 */
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 color;
};

/**
 * @class Mesh
 * @brief Representa una malla 3D con vértices, índices y texturas
 */
class Mesh {
public:
    /**
     * @brief Constructor
     * @param vertices Vector de vértices
     * @param indices Vector de índices
     * @param textures Vector de texturas (opcional)
     */
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
         const std::vector<Texture>& textures = {});

    /**
     * @brief Destructor - Limpia recursos OpenGL
     */
    ~Mesh();

    // Move semantics (necesario para almacenar en std::vector)
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    // Prevenir copia
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    /**
     * @brief Renderiza la malla, vinculando texturas si existen
     * @param shader Referencia al shader activo
     */
    void draw(const Shader& shader) const;

    /**
     * @brief Crea un cubo unitario con colores
     * @return unique_ptr al Mesh del cubo
     */
    static std::unique_ptr<Mesh> createCube();

    /**
     * @brief Obtiene la matriz de transformación
     * @return Matriz de modelo
     */
    glm::mat4 getModelMatrix() const { return modelMatrix; }

    /**
     * @brief Establece la matriz de transformación
     * @param matrix Nueva matriz de modelo
     */
    void setModelMatrix(const glm::mat4& matrix) { modelMatrix = matrix; }

    /**
     * @brief Comprueba si la malla tiene texturas
     * @return true si tiene texturas
     */
    bool hasTextures() const { return !textures.empty(); }

private:
    GLuint VAO, VBO, EBO;
    unsigned int indexCount;
    glm::mat4 modelMatrix;
    std::vector<Texture> textures;

    /**
     * @brief Configura los buffers de OpenGL
     */
    void setupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
};

}  // namespace Renderer

#endif  // MESH_HPP
