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

    // Posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

std::unique_ptr<Mesh> Mesh::createCube()
{
    std::vector<Vertex> vertices = { // Frontal (rojo)
                                     { { -0.5f, -0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f } },
                                     { { 0.5f, -0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f } },
                                     { { 0.5f, 0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f } },
                                     { { -0.5f, 0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f } },

                                     // Trasera (verde)
                                     { { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } },
                                     { { 0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } },
                                     { { 0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } },
                                     { { -0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } },

                                     // Superior (azul)
                                     { { -0.5f, 0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f } },
                                     { { 0.5f, 0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f } },
                                     { { 0.5f, 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } },
                                     { { -0.5f, 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } },

                                     // Inferior (amarillo)
                                     { { -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 0.0f } },
                                     { { 0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 0.0f } },
                                     { { 0.5f, -0.5f, 0.5f }, { 1.0f, 1.0f, 0.0f } },
                                     { { -0.5f, -0.5f, 0.5f }, { 1.0f, 1.0f, 0.0f } },

                                     // Derecha (magenta)
                                     { { 0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f } },
                                     { { 0.5f, 0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f } },
                                     { { 0.5f, 0.5f, 0.5f }, { 1.0f, 0.0f, 1.0f } },
                                     { { 0.5f, -0.5f, 0.5f }, { 1.0f, 0.0f, 1.0f } },

                                     // Izquierda (cyan)
                                     { { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f, 1.0f } },
                                     { { -0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f, 1.0f } },
                                     { { -0.5f, 0.5f, 0.5f }, { 0.0f, 1.0f, 1.0f } },
                                     { { -0.5f, -0.5f, 0.5f }, { 0.0f, 1.0f, 1.0f } }
    };

    std::vector<unsigned int> indices = {
        0,  1,  2,  2,  3,  0,   // Frontal
        4,  6,  5,  6,  4,  7,   // Trasera
        8,  9,  10, 10, 11, 8,   // Superior
        12, 14, 13, 14, 12, 15,  // Inferior
        16, 17, 18, 18, 19, 16,  // Derecha
        20, 22, 21, 22, 20, 23   // Izquierda
    };

    return std::make_unique<Mesh>(vertices, indices);
}

}  // namespace Renderer
