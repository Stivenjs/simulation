#version 330 core

// Atributos de entrada
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aColor;

// Uniforms - Matrices de transformación
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cellColor;

// Salida hacia el fragment shader
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec3 vertexColor;

void main() {
    // Posición en world space (para calcular iluminación)
    FragPos = vec3(model * vec4(aPos, 1.0));

    // Transformar normal al world space
    // La normal matrix corrige escalados no uniformes
    Normal = mat3(transpose(inverse(model))) * aNormal;

    // Pasar coordenadas de textura al fragment shader
    TexCoords = aTexCoords;

    // Color del vértice mezclado con color de celda
    vertexColor = mix(aColor, cellColor, 0.7);

    // Posición final en clip space
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
