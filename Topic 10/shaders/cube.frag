#version 330 core
out vec4 FragColor; // Returns FragColor

in vec3 Normal; // Receives normal
in vec3 FragPos; // Receives FragPos
in vec3 TexCoords; // Receives texture coordinates for cube mapping

uniform vec3 lightPos; // Receives lightPos uniform
uniform vec3 viewPos; // Receives viewPos uniform
uniform vec3 lightColor; // Receives lightColor uniform
uniform vec3 cubeColor; // Receives cubeColor uniform
uniform samplerCube skybox; // Cube map texture for environment mapping

void main() {
    // Environment mapping - sample cube map using reflected coordinates
    vec4 envColor = texture(skybox, TexCoords);
    
    // Basic lighting for enhanced realism
    float ambientStrength = 0.45; // Lighter than original (0.3)
    vec3 ambient = ambientStrength * lightColor;
    
    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Mix environment color with lighting
    vec3 result = envColor.rgb * (ambient + diffuse) + specular;
    FragColor = vec4(result, 1.0f);
}
