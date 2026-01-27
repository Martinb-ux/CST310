#version 330 core

// Input vertex attributes
layout (location = 0) in vec3 aPos;      // Vertex position
layout (location = 1) in vec3 aNormal;   // Vertex normal

// Output to fragment shader
out vec3 FragPos;    // Fragment position in world space
out vec3 Normal;     // Normal vector in world space

// Transformation matrices
uniform mat4 model;      // Model matrix (object to world space)
uniform mat4 view;       // View matrix (world to camera space)
uniform mat4 projection; // Projection matrix (camera to clip space)

void main()
{
    // Calculate fragment position in world space
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // Transform normal to world space
    // Use normal matrix to handle non-uniform scaling correctly
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    // Calculate final vertex position
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
