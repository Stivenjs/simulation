/**
 * @file Mesh.cpp
 * @brief Implementación de Mesh
 */

#include "Mesh.hpp"
#include "Shader.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Renderer {

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
           const std::vector<Texture>& textures) :
    VAO(0), VBO(0), EBO(0), indexCount(static_cast<unsigned int>(indices.size())), modelMatrix(glm::mat4(1.0f)),
    textures(textures)
{
    setupMesh(vertices, indices);
}

Mesh::~Mesh()
{
    if (VAO != 0)
        glDeleteVertexArrays(1, &VAO);
    if (VBO != 0)
        glDeleteBuffers(1, &VBO);
    if (EBO != 0)
        glDeleteBuffers(1, &EBO);
}

Mesh::Mesh(Mesh&& other) noexcept :
    VAO(other.VAO), VBO(other.VBO), EBO(other.EBO), indexCount(other.indexCount), modelMatrix(other.modelMatrix),
    textures(std::move(other.textures))
{
    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;
    other.indexCount = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    if (this != &other) {
        // Liberar recursos propios
        if (VAO != 0)
            glDeleteVertexArrays(1, &VAO);
        if (VBO != 0)
            glDeleteBuffers(1, &VBO);
        if (EBO != 0)
            glDeleteBuffers(1, &EBO);

        // Transferir ownership
        VAO = other.VAO;
        VBO = other.VBO;
        EBO = other.EBO;
        indexCount = other.indexCount;
        modelMatrix = other.modelMatrix;
        textures = std::move(other.textures);

        other.VAO = 0;
        other.VBO = 0;
        other.EBO = 0;
        other.indexCount = 0;
    }
    return *this;
}

void Mesh::draw(const Shader& shader) const
{
    // Indicar al shader si esta malla usa texturas
    shader.setBool("useTexture", !textures.empty());

    // Vincular texturas (si existen)
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);

        std::string number;
        std::string name = textures[i].type;

        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);

        shader.setInt(name + number, static_cast<int>(i));
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    glActiveTexture(GL_TEXTURE0);

    // Dibujar geometría
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
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)), vertices.data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)),
                 indices.data(), GL_STATIC_DRAW);

    // Posición (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    // Normal (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    // Coordenadas de textura (location = 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(2);

    // Color (location = 3)
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
}

std::unique_ptr<Mesh> Mesh::createCube()
{
    glm::vec3 white(1.0f);
    glm::vec2 uv(0.0f);  // Sin coordenadas de textura para cubo básico

    // Cada cara tiene su propia normal para iluminación correcta
    std::vector<Vertex> vertices = {
        // Cara frontal (normal +Z)
        { { -0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, uv, white },
        { { 0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, uv, white },
        { { 0.5f, 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, uv, white },
        { { -0.5f, 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, uv, white },

        // Cara trasera (normal -Z)
        { { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, uv, white },
        { { 0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, uv, white },
        { { 0.5f, 0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, uv, white },
        { { -0.5f, 0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, uv, white },

        // Cara superior (normal +Y)
        { { -0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, uv, white },
        { { 0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, uv, white },
        { { 0.5f, 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f }, uv, white },
        { { -0.5f, 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f }, uv, white },

        // Cara inferior (normal -Y)
        { { -0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f }, uv, white },
        { { 0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f }, uv, white },
        { { 0.5f, -0.5f, 0.5f }, { 0.0f, -1.0f, 0.0f }, uv, white },
        { { -0.5f, -0.5f, 0.5f }, { 0.0f, -1.0f, 0.0f }, uv, white },

        // Cara derecha (normal +X)
        { { 0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, uv, white },
        { { 0.5f, 0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, uv, white },
        { { 0.5f, 0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f }, uv, white },
        { { 0.5f, -0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f }, uv, white },

        // Cara izquierda (normal -X)
        { { -0.5f, -0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f }, uv, white },
        { { -0.5f, 0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f }, uv, white },
        { { -0.5f, 0.5f, 0.5f }, { -1.0f, 0.0f, 0.0f }, uv, white },
        { { -0.5f, -0.5f, 0.5f }, { -1.0f, 0.0f, 0.0f }, uv, white },
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
