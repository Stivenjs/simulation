/**
 * @file Model.cpp
 * @brief Implementación de carga de modelos 3D con Assimp
 *
 * Flujo de carga:
 *   1. Assimp lee el archivo y genera un aiScene
 *   2. Se recorre el árbol de nodos recursivamente (processNode)
 *   3. Cada aiMesh se convierte a nuestro Mesh (processMesh)
 *   4. Las texturas se cargan con cache (loadMaterialTextures)
 */

#include "Model.hpp"
#include "renderer/Shader.hpp"
#include "renderer/Texture.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <iostream>

namespace Renderer {

Model::Model(const std::string& path)
{
    loadModel(path);
}

void Model::draw(const Shader& shader) const
{
    for (const auto& mesh : meshes) {
        mesh.draw(shader);
    }
}

void Model::loadModel(const std::string& path)
{
    std::cout << "Loading model: " << path << std::endl;

    Assimp::Importer importer;

    // Flags de post-procesado:
    //   Triangulate     - Convertir todas las caras a triángulos
    //   GenSmoothNormals - Generar normales suaves si no existen
    //   FlipUVs         - Voltear coordenadas UV en Y (OpenGL convention)
    //   CalcTangentSpace - Calcular tangentes (útil para normal mapping futuro)
    //   OptimizeMeshes  - Combinar mallas pequeñas para menos draw calls
    const aiScene* scene
        = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs
                                      | aiProcess_CalcTangentSpace | aiProcess_OptimizeMeshes);

    if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    // Extraer directorio del archivo (compatible Windows y Unix)
    size_t lastSlash = path.find_last_of("/\\");
    if (lastSlash != std::string::npos)
        directory = path.substr(0, lastSlash);
    else
        directory = ".";

    // Procesar el árbol de nodos recursivamente
    processNode(scene->mRootNode, scene);

    std::cout << "  Model loaded: " << meshes.size() << " meshes, " << texturesLoaded.size() << " textures"
              << std::endl;
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // Procesar cada malla en este nodo
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    // Procesar nodos hijos recursivamente
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // --- Vértices ---
    vertices.reserve(mesh->mNumVertices);

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex {};

        // Posición
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        // Normal
        if (mesh->HasNormals()) {
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
        } else {
            vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
        }

        // Coordenadas de textura (primer set - Assimp soporta hasta 8)
        if (mesh->mTextureCoords[0]) {
            vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
        } else {
            vertex.texCoords = glm::vec2(0.0f);
        }

        // Color de vértice
        if (mesh->HasVertexColors(0)) {
            vertex.color.x = mesh->mColors[0][i].r;
            vertex.color.y = mesh->mColors[0][i].g;
            vertex.color.z = mesh->mColors[0][i].b;
        } else {
            vertex.color = glm::vec3(1.0f);  // Blanco por defecto
        }

        vertices.push_back(vertex);
    }

    // --- Índices ---
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // --- Materiales / Texturas ---
    if (mesh->mMaterialIndex < scene->mNumMaterials) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // Texturas diffuse (color base)
        auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        // Texturas specular (brillo/reflejo)
        auto specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName)
{
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        // Verificar si ya fue cargada (cache para evitar duplicados)
        bool alreadyLoaded = false;
        for (const auto& loaded : texturesLoaded) {
            if (loaded.path == str.C_Str()) {
                textures.push_back(loaded);
                alreadyLoaded = true;
                break;
            }
        }

        if (!alreadyLoaded) {
            Texture texture;
            texture.id = TextureLoader::loadFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            texturesLoaded.push_back(texture);  // Agregar al cache
        }
    }

    return textures;
}

}  // namespace Renderer
