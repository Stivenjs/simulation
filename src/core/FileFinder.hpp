/**
 * @file FileFinder.hpp
 * @brief Utilidad para buscar archivos en el proyecto de forma recursiva
 *
 * Busca archivos por nombre o patrón en múltiples ubicaciones:
 * - Directorio actual de trabajo
 * - Directorio del ejecutable
 * - Búsqueda recursiva con profundidad limitada
 */

#ifndef FILE_FINDER_HPP
#define FILE_FINDER_HPP

#include <string>
#include <vector>

namespace Core {

/**
 * @class FileFinder
 * @brief Utilidad estática para buscar archivos en el proyecto
 */
class FileFinder {
public:
    /**
     * @brief Busca un archivo por nombre o ruta relativa
     * @param filename Nombre del archivo (ej: "mi_modelo.obj") o ruta (ej: "models/casa.obj")
     * @return Ruta completa al archivo encontrado, o filename si no se encuentra
     */
    static std::string findFile(const std::string& filename);

    /**
     * @brief Busca todos los archivos que coinciden con una extensión en una carpeta
     * @param directory Carpeta donde buscar (ej: "models")
     * @param extensions Extensiones a buscar (ej: {".obj", ".fbx", ".gltf"})
     * @return Vector con las rutas completas de todos los archivos encontrados
     */
    static std::vector<std::string> findFilesInDirectory(const std::string& directory,
                                                         const std::vector<std::string>& extensions);

    /**
     * @brief Obtiene el directorio del ejecutable
     * @return Ruta al directorio del ejecutable
     */
    static std::string getExecutableDirectory();

private:
    /**
     * @brief Búsqueda recursiva por nombre de archivo
     * @param dir Directorio donde buscar
     * @param filename Nombre del archivo a buscar
     * @param maxDepth Profundidad máxima de búsqueda
     * @param currentDepth Profundidad actual
     * @return Ruta al archivo encontrado, o string vacío si no se encuentra
     */
    static std::string searchRecursive(const std::string& dir, const std::string& filename, int maxDepth,
                                       int currentDepth = 0);
};

}  // namespace Core

#endif  // FILE_FINDER_HPP
