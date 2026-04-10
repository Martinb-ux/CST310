#version 330 core
out vec4 FragColor; // Returns FragColor

in vec3 Normal; // Receives Normal
in vec3 FragPos; // Receives FragPos
in vec2 TexCoords; // Receives texture coordinates
in vec3 TangentLightPos; // Light position in tangent space
in vec3 TangentViewPos; // View position in tangent space
in vec3 TangentFragPos; // Fragment position in tangent space
  
uniform vec3 lightPos; // Light position
uniform vec3 viewPos; // Camera position
uniform vec3 lightColor; // Light color
uniform vec3 cylinderColor; // Cylinder color
uniform sampler2D diffuseMap; // Diffuse texture
uniform sampler2D normalMap; // Normal map for bump mapping

void main()
{
    // Sample textures
    vec4 diffuseColor = texture(diffuseMap, TexCoords);
    
    // Get normal from normal map
    vec3 normal = texture(normalMap, TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0); // Transform from [0,1] to [-1,1]
    
    // Calculate lighting in tangent space
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    
    // Ambient
    float ambientStrength = 0.6; // Increased for brighter cylinder
    vec3 ambient = ambientStrength * lightColor;
  	
    // Diffuse with bumped normal
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular with bumped normal
    float specularStrength = 0.5;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * lightColor;
        
    // Combine lighting with texture
    vec3 result = (ambient + diffuse + specular) * diffuseColor.rgb;
    FragColor = vec4(result, 1.0f);
} 
