#version 330 core
out vec4 FragColor; // Returns FragColor

in vec3 FragPos; // Receives FragPos
in vec3 Normal; // Receives Normal
in vec2 TexCoords; // Receives texture coordinates
in vec3 TangentLightPos; // Light position in tangent space
in vec3 TangentViewPos; // View position in tangent space
in vec3 TangentFragPos; // Fragment position in tangent space

uniform vec3 lightPos; // Light position
uniform vec3 viewPos; // Camera position
uniform vec3 lightColor; // Light color
uniform vec3 sphereColor; // Sphere color
uniform sampler2D diffuseMap; // Diffuse texture
uniform sampler2D heightMap; // Height map for parallax mapping
uniform float heightScale; // Height scale for parallax effect

// Simplified Parallax Mapping function (less noise)
vec2 parallaxMapping(vec2 texCoords, vec3 viewDir) {
    // Simple parallax mapping with fewer layers to reduce noise
    const float numLayers = 16.0;
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;
    
    // Reduce the parallax effect for smoother results
    vec2 P = viewDir.xy * heightScale * 0.5;
    vec2 deltaTexCoords = P / numLayers;
    
    vec2 currentTexCoords = texCoords;
    float currentDepthMapValue = texture(heightMap, currentTexCoords).r;
    
    // Add early exit to prevent artifacts
    if (length(viewDir.xy) < 0.1) {
        return texCoords;
    }
    
    while (currentLayerDepth < currentDepthMapValue) {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = texture(heightMap, currentTexCoords).r;
        currentLayerDepth += layerDepth;
        
        // Prevent infinite loop
        if (currentTexCoords.x < 0.0 || currentTexCoords.x > 1.0 ||
            currentTexCoords.y < 0.0 || currentTexCoords.y > 1.0) {
            return texCoords;
        }
    }
    
    return currentTexCoords;
}

void main() {
    // Calculate view direction in tangent space
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    
    // Apply parallax mapping
    vec2 displacedTexCoords = parallaxMapping(TexCoords, viewDir);
    
    // Check if displaced coordinates are valid
    if (displacedTexCoords.x > 1.0 || displacedTexCoords.y > 1.0 || displacedTexCoords.x < 0.0 || displacedTexCoords.y < 0.0) {
        discard;
    }
    
    // Sample textures
    vec4 diffuseColor = texture(diffuseMap, displacedTexCoords);
    
    // Lighting calculations in tangent space
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    
    // Ambient
    float ambientStrength = 0.15; // Darker than current (0.3)
    vec3 ambient = ambientStrength * lightColor;
    
    // Diffuse
    float diff = max(dot(lightDir, viewDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularStrength = 0.5;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * lightColor;
    
    // Combine lighting with texture
    vec3 result = (ambient + diffuse + specular) * diffuseColor.rgb;
    FragColor = vec4(result, 1.0);
}
