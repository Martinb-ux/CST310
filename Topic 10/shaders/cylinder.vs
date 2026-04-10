#version 330 core
layout (location = 0) in vec3 aPos; // Receives aPos
layout (location = 2) in vec2 aTexCoords; // Receives texture coordinates
layout (location = 4) in vec3 aNormal; // Receives aNormal

out vec3 FragPos; // Returns FragPos
out vec3 Normal; // Returns Normal
out vec2 TexCoords; // Returns texture coordinates
out vec3 TangentLightPos; // Light position in tangent space
out vec3 TangentViewPos; // View position in tangent space
out vec3 TangentFragPos; // Fragment position in tangent space

uniform mat4 model; // Receives model uniform
uniform mat4 view; // Receives view uniform
uniform mat4 projection; // Receives projection uniform
uniform vec3 lightPos; // Light position
uniform vec3 viewPos; // Camera position

// Calculate tangent space for cylinder
vec3 calculateTangent(vec3 normal, vec3 position) {
    // For cylinder, tangent is typically around the circular direction
    vec3 worldPos = vec3(model * vec4(position, 1.0));
    vec3 worldNormal = normalize(mat3(transpose(inverse(model))) * normal);
    
    // Calculate tangent based on cylinder geometry
    vec3 tangent;
    if (abs(worldNormal.y) > 0.9) {
        // If normal points up/down, use x-axis as tangent
        tangent = normalize(vec3(worldPos.z, 0.0, -worldPos.x));
    } else {
        // Otherwise, calculate tangent around cylinder
        tangent = normalize(vec3(-worldPos.z, 0.0, worldPos.x));
    }
    
    // Ensure tangent is perpendicular to normal
    tangent = normalize(tangent - dot(tangent, worldNormal) * worldNormal);
    return tangent;
}

void main()
{
    gl_Position = projection * view * vec4(aPos, 1.0f);  // Implements transformations
    FragPos = vec3(model * vec4(aPos, 1.0));  // Sets fragment position
    Normal = mat3(transpose(inverse(model))) * aNormal;  // Normalizes
    TexCoords = aTexCoords; // Pass texture coordinates
    
    // Calculate tangent space for bump mapping
    vec3 T = calculateTangent(aNormal, aPos);
    vec3 N = normalize(Normal);
    vec3 B = cross(N, T);
    
    mat3 TBN = mat3(T, B, N);
    
    // Transform positions to tangent space
    TangentLightPos = TBN * lightPos;
    TangentViewPos = TBN * viewPos;
    TangentFragPos = TBN * FragPos;
}
