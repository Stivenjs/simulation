/**
 * @file Model.hpp
 * @brief Carga de modelos 3D con Assimp
 *
 * Soporta formatos: OBJ, FBX, GLTF, BLEND, DAE, 3DS, STL, y más.
 * Un Model contiene múltiples Meshes y gestiona texturas con cache.
 *
 * Uso:
 *   auto model = std::make_unique<Renderer::Model>("models/robot.obj");
 *   model->draw(shader);
 */

#ifndef MODEL_HPP
#define MODEL_HPP

#include "renderer/Mesh.hpp"
#include "renderer/Texture.hpp"

#include <assimp/scene.h>
#include <string>
#include <vector>

namespace Renderer {

// Forward declaration
class Shader;

/**
 * @class Model
 * @brief Carga y renderiza modelos 3D desde archivos
 */
class Model {
public:
    /**
     * @brief Carga un modelo desde un archivo
     * @param path Ruta al archivo del modelo (.obj, .fbx, .gltf, etc.)
     */
    explicit Model(const std::string& path);

    /**
     * @brief Renderiza todas las mallas del modelo
     * @param shader Referencia al shader activo
     */
    void draw(const Shader& shader) const;

    /**
     * @brief Obtiene el número de mallas en el modelo
     * @return Cantidad de meshes
     */
    int getMeshCount() const { return static_cast<int>(meshes.size()); }

    /**
     * @brief Obtiene el número de texturas cargadas
     * @return Cantidad de texturas únicas
     */
    int getTextureCount() const { return static_cast<int>(texturesLoaded.size()); }

private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> texturesLoaded;  // Cache de texturas ya cargadas

    /**
     * @brief Carga el modelo usando Assimp
     * @param path Ruta al archivo
     */
    void loadModel(const std::string& path);

    /**
     * @brief Procesa un nodo del árbol de Assimp (recursivo)
     * @param node Nodo actual
     * @param scene Escena de Assimp
     */
    void processNode(aiNode* node, const aiScene* scene);

    /**
     * @brief Convierte un aiMesh de Assimp a nuestro Mesh
     * @param mesh Malla de Assimp
     * @param scene Escena de Assimp
     * @return Mesh propio con los datos convertidos
     */
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    /**
     * @brief Carga las texturas de un material (con cache)
     * @param mat Material de Assimp
     * @param type Tipo de textura (diffuse, specular, etc.)
     * @param typeName Nombre del tipo como string
     * @return Vector de texturas cargadas
     */
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
};

}  // namespace Renderer

#endif  // MODEL_HPP
