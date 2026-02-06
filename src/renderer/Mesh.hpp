/**
 * @file Mesh.hpp
 * @brief Representación de geometría 3D
 *
 * Similar a THREE.Mesh en Three.js
 */

#ifndef MESH_HPP
#define MESH_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace Renderer {

/**
 * @struct Vertex
 * @brief Estructura de un vértice con posición, normal y color
 */
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
};

/**
 * @class Mesh
 * @brief Representa una malla 3D con vértices e índices
 */
class Mesh {
public:
    /**
     * @brief Constructor
     * @param vertices Vector de vértices
     * @param indices Vector de índices
     */
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

    /**
     * @brief Destructor - Limpia recursos OpenGL
     */
    ~Mesh();

    // Prevenir copia
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    /**
     * @brief Renderiza la malla
     */
    void draw() const;

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

private:
    GLuint VAO, VBO, EBO;
    unsigned int indexCount;
    glm::mat4 modelMatrix;

    /**
     * @brief Configura los buffers de OpenGL
     */
    void setupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
};

}  // namespace Renderer

#endif  // MESH_HPP
