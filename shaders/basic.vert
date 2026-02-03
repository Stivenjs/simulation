#version 330 core

// Atributos de entrada
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

// Uniforms - Matrices de transformación
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cellColor; // Color dinámico por celda

// Salida hacia el fragment shader
out vec3 vertexColor;

void main() {
    // Aplicar transformaciones MVP
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    // Mezclar color del vértice con color de la celda
    vertexColor = mix(aColor, cellColor, 0.7);
}
