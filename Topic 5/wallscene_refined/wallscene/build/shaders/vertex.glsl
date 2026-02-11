/**
 * Vertex Shader for Stairwell Scene
 *
 * Purpose:
 * - Transforms vertex positions from model space to clip space
 * - Passes vertex colors and normals to fragment shader
 * - Calculates world-space position for lighting
 *
 * Mathematical Operations:
 * - Model Matrix: Transforms from local object space to world space
 *   (includes translation, rotation, scaling)
 * - View Matrix: Transforms from world space to camera/view space
 * - Projection Matrix: Transforms from view space to clip space
 *   (perspective division happens automatically)
 *
 * gl_Position = Projection * View * Model * vertexPosition
 */

#version 330 core

// Input vertex attributes
layout (location = 0) in vec3 aPos;      // Vertex position in model space
layout (location = 1) in vec3 aColor;    // Vertex color (RGB)
layout (location = 2) in vec3 aNormal;   // Vertex normal for lighting

// Output to fragment shader
out vec3 FragPos;    // Fragment position in world space
out vec3 Normal;     // Normal vector in world space
out vec3 Color;      // Vertex color passed through

// Uniform transformation matrices
uniform mat4 model;       // Model matrix (object -> world)
uniform mat4 view;        // View matrix (world -> camera)
uniform mat4 projection;  // Projection matrix (camera -> clip)

void main() {
    // Calculate world-space position for lighting calculations
    // FragPos = (Model * vec4(aPos, 1.0)).xyz
    FragPos = vec3(model * vec4(aPos, 1.0));

    // Transform normal to world space
    // Note: For non-uniform scaling, should use inverse transpose of model matrix
    // For this project, we use uniform scaling, so this simplification works
    Normal = mat3(transpose(inverse(model))) * aNormal;

    // Pass color through to fragment shader
    Color = aColor;

    // Calculate final clip-space position
    // This is the standard MVP (Model-View-Projection) transformation
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
