#version 330 core
layout (location = 0) in vec3 aPos; // Receives aPos
layout (location = 1) in vec3 aNormal; // Receives aNormal
layout (location = 2) in vec2 aTexCoords; // Receives texture coordinates

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

void main() {
    gl_Position = projection * view * vec4(aPos, 1.0f);  // Implements transformations
    FragPos = vec3(model * vec4(aPos, 1.0));  // Sets fragment position
    Normal = mat3(transpose(inverse(model))) * aNormal;  // Normalizes
    TexCoords = aTexCoords; // Pass texture coordinates
    
    // Calculate tangent space for parallax mapping
    vec3 N = normalize(Normal);
    vec3 T = normalize(vec3(-N.z, 0.0, N.x)); // Calculate tangent
    vec3 B = cross(N, T);
    
    // Ensure orthonormal basis
    T = normalize(T - dot(T, N) * N);
    B = normalize(B - dot(B, N) * N - dot(B, T) * T);
    
    mat3 TBN = mat3(T, B, N);
    
    // Transform positions to tangent space
    TangentLightPos = TBN * lightPos;
    TangentViewPos = TBN * viewPos;
    TangentFragPos = TBN * FragPos;
}
