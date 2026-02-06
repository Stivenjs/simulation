/**
 * @file FileFinder.cpp
 * @brief Implementación de FileFinder
 */

#include "FileFinder.hpp"
#include <algorithm>
#include <filesystem>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <limits.h>
#include <unistd.h>
#endif

namespace Core {

namespace fs = std::filesystem;

std::string FileFinder::getExecutableDirectory()
{
#ifdef _WIN32
    char exePath[MAX_PATH];
    if (GetModuleFileNameA(nullptr, exePath, MAX_PATH) > 0) {
        std::string exeDir = fs::path(exePath).parent_path().string();
        std::replace(exeDir.begin(), exeDir.end(), '\\', '/');
        return exeDir;
    }
#else
    char exePath[PATH_MAX];
    if (readlink("/proc/self/exe", exePath, PATH_MAX) != -1) {
        return fs::path(exePath).parent_path().string();
    }
#endif
    return "";
}

std::string FileFinder::searchRecursive(const std::string& dir, const std::string& filename, int maxDepth,
                                        int currentDepth)
{
    if (currentDepth > maxDepth)
        return {};

    try {
        for (const auto& entry : fs::directory_iterator(dir)) {
            if (entry.is_directory()) {
                std::string found = searchRecursive(entry.path().string(), filename, maxDepth, currentDepth + 1);
                if (!found.empty())
                    return found;
            } else if (entry.path().filename().string() == filename) {
                std::string result = entry.path().string();
                std::replace(result.begin(), result.end(), '\\', '/');
                return result;
            }
        }
    } catch (...) {
    }

    return {};
}

std::string FileFinder::findFile(const std::string& filename)
{
    std::string pathNorm = filename;
    std::replace(pathNorm.begin(), pathNorm.end(), '\\', '/');

    // 1) Rutas directas: path tal cual, cwd + path, exeDir + path, exeDir/.. + path
    std::vector<std::string> candidates = { pathNorm };

    try {
        std::string cwd = fs::current_path().string();
        std::replace(cwd.begin(), cwd.end(), '\\', '/');
        candidates.push_back(cwd + "/" + pathNorm);
    } catch (...) {
    }

    std::string exeDir = getExecutableDirectory();
    if (!exeDir.empty()) {
        candidates.push_back(exeDir + "/" + pathNorm);
        candidates.push_back(exeDir + "/../" + pathNorm);
    }

    // Verificar si existe
    for (const auto& p : candidates) {
        try {
            if (fs::exists(p) && fs::is_regular_file(p)) {
                return p;
            }
        } catch (...) {
        }
    }

    // 2) Búsqueda recursiva por nombre de archivo (max 5 niveles)
    const std::string fileOnly = fs::path(pathNorm).filename().string();
    const int maxDepth = 5;

    try {
        std::string found = searchRecursive(fs::current_path().string(), fileOnly, maxDepth);
        if (!found.empty())
            return found;
    } catch (...) {
    }

    if (!exeDir.empty()) {
        try {
            std::string found = searchRecursive(exeDir, fileOnly, maxDepth);
            if (!found.empty())
                return found;
        } catch (...) {
        }
        try {
            std::string parentDir = fs::path(exeDir).parent_path().string();
            std::string found = searchRecursive(parentDir, fileOnly, maxDepth);
            if (!found.empty())
                return found;
        } catch (...) {
        }
    }

    return pathNorm;
}

std::vector<std::string> FileFinder::findFilesInDirectory(const std::string& directory,
                                                          const std::vector<std::string>& extensions)
{
    std::vector<std::string> foundFiles;

    // Buscar la carpeta primero
    std::string dirPath = findFile(directory);

    // Si findFile devuelve el mismo string, significa que no encontró la carpeta
    // Intentar como directorio directamente
    if (dirPath == directory) {
        // Intentar rutas comunes
        std::vector<std::string> dirCandidates = { directory };

        try {
            std::string cwd = fs::current_path().string();
            std::replace(cwd.begin(), cwd.end(), '\\', '/');
            dirCandidates.push_back(cwd + "/" + directory);
        } catch (...) {
        }

        std::string exeDir = getExecutableDirectory();
        if (!exeDir.empty()) {
            dirCandidates.push_back(exeDir + "/" + directory);
            dirCandidates.push_back(exeDir + "/../" + directory);
        }

        for (const auto& candidate : dirCandidates) {
            try {
                if (fs::exists(candidate) && fs::is_directory(candidate)) {
                    dirPath = candidate;
                    break;
                }
            } catch (...) {
            }
        }
    }

    // Si no encontramos la carpeta, buscar recursivamente
    if (dirPath == directory || !fs::exists(dirPath) || !fs::is_directory(dirPath)) {
        // Buscar recursivamente cualquier carpeta con ese nombre
        const int maxDepth = 5;
        try {
            std::string foundDir = searchRecursive(fs::current_path().string(), directory, maxDepth);
            if (!foundDir.empty() && fs::is_directory(foundDir)) {
                dirPath = foundDir;
            }
        } catch (...) {
        }
    }

    // Si aún no encontramos, devolver vacío
    if (!fs::exists(dirPath) || !fs::is_directory(dirPath)) {
        return foundFiles;
    }

    // Buscar todos los archivos con las extensiones especificadas
    try {
        for (const auto& entry : fs::recursive_directory_iterator(dirPath)) {
            if (entry.is_regular_file()) {
                std::string ext = entry.path().extension().string();
                std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

                for (const auto& wantedExt : extensions) {
                    std::string wantedExtLower = wantedExt;
                    std::transform(wantedExtLower.begin(), wantedExtLower.end(), wantedExtLower.begin(), ::tolower);

                    if (ext == wantedExtLower) {
                        std::string filePath = entry.path().string();
                        std::replace(filePath.begin(), filePath.end(), '\\', '/');
                        foundFiles.push_back(filePath);
                        break;  // Solo agregar una vez
                    }
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error buscando archivos en " << dirPath << ": " << e.what() << std::endl;
    }

    return foundFiles;
}

}  // namespace Core
