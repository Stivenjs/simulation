/**
 * @file Texture.hpp
 * @brief Carga y gestión de texturas OpenGL
 *
 * Proporciona utilidades para cargar imágenes como texturas GPU.
 * Soporta formatos: PNG, JPG, BMP, TGA, etc. mediante stb_image.
 */

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GL/glew.h>
#include <string>

namespace Renderer {

/**
 * @struct Texture
 * @brief Representa una textura OpenGL cargada
 */
struct Texture {
    GLuint id = 0;
    std::string type;  // "texture_diffuse", "texture_specular"
    std::string path;  // Ruta del archivo (para cache)
};

/**
 * @class TextureLoader
 * @brief Utilidad para cargar texturas desde archivos de imagen
 */
class TextureLoader {
public:
    /**
     * @brief Carga una textura desde un archivo relativo a un directorio
     * @param filename Nombre del archivo de imagen
     * @param directory Directorio base
     * @return ID de la textura OpenGL generada (0 si falla)
     */
    static GLuint loadFromFile(const std::string& filename, const std::string& directory);

    /**
     * @brief Carga una textura desde una ruta completa
     * @param fullPath Ruta completa al archivo de imagen
     * @return ID de la textura OpenGL generada (0 si falla)
     */
    static GLuint loadFromFile(const std::string& fullPath);

private:
    /**
     * @brief Genera una textura OpenGL a partir de datos raw de imagen
     * @param data Puntero a los datos de la imagen
     * @param width Ancho en píxeles
     * @param height Alto en píxeles
     * @param channels Número de canales de color
     * @return ID de la textura generada
     */
    static GLuint generateTexture(unsigned char* data, int width, int height, int channels);
};

}  // namespace Renderer

#endif  // TEXTURE_HPP
