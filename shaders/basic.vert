#version 330 core

// Atributos de entrada
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

// Salida hacia el fragment shader
out vec3 vertexColor;

void main() {
    gl_Position = vec4(aPos, 1.0);
    vertexColor = aColor;
}
