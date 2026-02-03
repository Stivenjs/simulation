/**
 * @file Shader.hpp
 * @brief Gestión de shaders OpenGL
 *
 * Carga, compila y enlaza vertex y fragment shaders.
 */

#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>
#include <string>
#include <memory>

namespace Renderer {

/**
 * @class Shader
 * @brief Clase para gestionar programas de shader OpenGL
 */
class Shader {
public:
  /**
   * @brief Constructor desde código fuente directo
   * @param vertexSource Código fuente del vertex shader
   * @param fragmentSource Código fuente del fragment shader
   */
  Shader(const std::string &vertexSource, const std::string &fragmentSource);

  /**
   * @brief Carga shaders desde archivos
   * @param vertexPath Ruta al archivo vertex shader (.vert)
   * @param fragmentPath Ruta al archivo fragment shader (.frag)
   * @return unique_ptr al Shader cargado desde archivos
   */
  static std::unique_ptr<Shader> fromFiles(const std::string &vertexPath,
                                            const std::string &fragmentPath);

  /**
   * @brief Destructor - Limpia recursos OpenGL
   */
  ~Shader();

  // Prevenir copia
  Shader(const Shader &) = delete;
  Shader &operator=(const Shader &) = delete;

  /**
   * @brief Activa este shader para renderizado
   */
  void use() const;

  /**
   * @brief Desactiva el shader
   */
  void unuse() const;

  /**
   * @brief Obtiene el ID del programa shader
   * @return ID del programa OpenGL
   */
  GLuint getProgram() const { return program; }

  /**
   * @brief Establece un uniform float
   * @param name Nombre del uniform
   * @param value Valor a establecer
   */
  void setFloat(const std::string &name, float value) const;

  /**
   * @brief Establece un uniform vec3
   * @param name Nombre del uniform
   * @param x Componente x
   * @param y Componente y
   * @param z Componente z
   */
  void setVec3(const std::string &name, float x, float y, float z) const;

  /**
   * @brief Establece un uniform mat4
   * @param name Nombre del uniform
   * @param matrix Puntero a la matriz 4x4
   */
  void setMat4(const std::string &name, const float *matrix) const;

private:
  GLuint program;

  /**
   * @brief Lee el contenido de un archivo
   * @param filepath Ruta al archivo
   * @return Contenido del archivo como string
   */
  static std::string readFile(const std::string &filepath);

  /**
   * @brief Compila un shader individual
   * @param source Código fuente del shader
   * @param type Tipo de shader (GL_VERTEX_SHADER o GL_FRAGMENT_SHADER)
   * @return ID del shader compilado
   */
  GLuint compileShader(const std::string &source, GLenum type);

  /**
   * @brief Enlaza vertex y fragment shaders en un programa
   * @param vertexShader ID del vertex shader
   * @param fragmentShader ID del fragment shader
   */
  void linkProgram(GLuint vertexShader, GLuint fragmentShader);

  /**
   * @brief Verifica errores de compilación/linkeo
   * @param shader ID del shader o programa
   * @param type Tipo de verificación
   */
  void checkCompileErrors(GLuint shader, const std::string &type);
};

} // namespace Renderer

#endif // SHADER_HPP
