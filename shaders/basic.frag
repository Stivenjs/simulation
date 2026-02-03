#version 330 core

// Entrada desde el vertex shader
in vec3 vertexColor;

// Salida final del color
out vec4 FragColor;

void main() {
    FragColor = vec4(vertexColor, 1.0);
}
