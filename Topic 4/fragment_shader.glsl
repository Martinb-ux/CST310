#version 330 core

// Input from vertex shader
in vec3 FragPos;   // Fragment position in world space
in vec3 Normal;    // Normal vector in world space

// Output color
out vec4 FragColor;

// Uniforms for lighting
uniform vec3 lightPos;     // Position of the light source
uniform vec3 viewPos;      // Position of the camera/viewer
uniform vec3 lightColor;   // Color of the light
uniform vec3 objectColor;  // Base color of the object

void main()
{
    // Ambient lighting component
    // Provides base illumination even in shadowed areas
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;
    
    // Diffuse lighting component
    // Simulates directional light scattering off the surface
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular lighting component
    // Creates shiny highlights on surfaces
    float specularStrength = 0.4;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    
    // Combine all lighting components
    vec3 result = (ambient + diffuse + specular) * objectColor;
    
    // Output final color
    FragColor = vec4(result, 1.0);
}
