#version 330 core
layout (location = 0) in vec3 aPos; // Receives aPos
layout (location = 1) in vec3 aNormal; // Receives aNormal

out vec3 FragPos; // Returns FragPos
out vec3 Normal; // Returns Normal
out vec3 TexCoords; // Returns texture coordinates for cube mapping

uniform mat4 model; // Receives model uniform
uniform mat4 view; // Receives view uniform
uniform mat4 projection; // Receives projection uniform
uniform vec3 viewPos; // Camera position for environment mapping

void main() {
    gl_Position = projection * view * vec4(aPos, 1.0f);  // Implements transformations
    FragPos = vec3(model * vec4(aPos, 1.0));  // Sets fragment position
    Normal = mat3(transpose(inverse(model))) * aNormal;  // Normalizes
    
    // Calculate texture coordinates for cube mapping
    vec3 I = normalize(FragPos - viewPos);
    vec3 N = normalize(Normal);
    TexCoords = reflect(I, N);
}
