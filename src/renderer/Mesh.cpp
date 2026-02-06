/**
 * @file Mesh.cpp
 * @brief Implementación de Mesh
 */

#include "Mesh.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Renderer {

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) :
    VAO(0), VBO(0), EBO(0), indexCount(indices.size()), modelMatrix(glm::mat4(1.0f))
{
    setupMesh(vertices, indices);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::draw() const
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Posición (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    // Color (location = 2)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

std::unique_ptr<Mesh> Mesh::createCube()
{
    // Color base blanco - el color real se controla por uniform cellColor
    glm::vec3 white(1.0f);

    // Cada cara tiene su propia normal para iluminación correcta
    std::vector<Vertex> vertices = {
        // Cara frontal (normal +Z)
        { { -0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, white },
        { { 0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, white },
        { { 0.5f, 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, white },
        { { -0.5f, 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, white },

        // Cara trasera (normal -Z)
        { { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, white },
        { { 0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, white },
        { { 0.5f, 0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, white },
        { { -0.5f, 0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, white },

        // Cara superior (normal +Y)
        { { -0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, white },
        { { 0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, white },
        { { 0.5f, 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f }, white },
        { { -0.5f, 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f }, white },

        // Cara inferior (normal -Y)
        { { -0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f }, white },
        { { 0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f }, white },
        { { 0.5f, -0.5f, 0.5f }, { 0.0f, -1.0f, 0.0f }, white },
        { { -0.5f, -0.5f, 0.5f }, { 0.0f, -1.0f, 0.0f }, white },

        // Cara derecha (normal +X)
        { { 0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, white },
        { { 0.5f, 0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, white },
        { { 0.5f, 0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f }, white },
        { { 0.5f, -0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f }, white },

        // Cara izquierda (normal -X)
        { { -0.5f, -0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f }, white },
        { { -0.5f, 0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f }, white },
        { { -0.5f, 0.5f, 0.5f }, { -1.0f, 0.0f, 0.0f }, white },
        { { -0.5f, -0.5f, 0.5f }, { -1.0f, 0.0f, 0.0f }, white },
    };

    std::vector<unsigned int> indices = {
        0,  1,  2,  2,  3,  0,   // Frontal
        4,  6,  5,  6,  4,  7,   // Trasera
        8,  9,  10, 10, 11, 8,   // Superior
        12, 14, 13, 14, 12, 15,  // Inferior
        16, 17, 18, 18, 19, 16,  // Derecha
        20, 22, 21, 22, 20, 23,  // Izquierda
    };

    return std::make_unique<Mesh>(vertices, indices);
}

}  // namespace Renderer
