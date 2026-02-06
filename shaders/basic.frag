#version 330 core

// Máximo de luces puntuales
#define MAX_POINT_LIGHTS 4

// Estructuras de iluminación
struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

// Entrada desde vertex shader
in vec3 FragPos;
in vec3 Normal;
in vec3 vertexColor;

// Uniforms
uniform DirectionalLight dirLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int numPointLights;
uniform Material material;
uniform vec3 viewPos;

// Salida final
out vec4 FragColor;

// Prototipos
vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    // Normalizar la normal interpolada
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // === Fase 1: Luz direccional ===
    vec3 result = CalcDirectionalLight(dirLight, norm, viewDir);

    // === Fase 2: Luces puntuales ===
    for (int i = 0; i < numPointLights && i < MAX_POINT_LIGHTS; i++) {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    // Aplicar color del vértice/celda
    result *= vertexColor;

    FragColor = vec4(result, 1.0);
}

/**
 * Calcula la contribución de una luz direccional
 * usando el modelo Phong: ambient + diffuse + specular
 */
vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);

    // Componente difusa (cuánta luz recibe la superficie)
    float diff = max(dot(normal, lightDir), 0.0);

    // Componente especular (reflejo brillante)
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Combinar componentes
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;

    return ambient + diffuse + specular;
}

/**
 * Calcula la contribución de una luz puntual
 * con atenuación por distancia
 */
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    // Componente difusa
    float diff = max(dot(normal, lightDir), 0.0);

    // Componente especular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Atenuación (la luz se debilita con la distancia)
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Combinar componentes con atenuación
    vec3 ambient = light.ambient * material.ambient * attenuation;
    vec3 diffuse = light.diffuse * diff * material.diffuse * attenuation;
    vec3 specular = light.specular * spec * material.specular * attenuation;

    return ambient + diffuse + specular;
}
