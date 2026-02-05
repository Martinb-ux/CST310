/**
 * Project 4: Stairwell Scene - OpenGL Rendering (Realistic Version)
 *
 * Authentic industrial stairwell with:
 * - Correct door anatomy (handle position, offset window, metal material)
 * - Imperfect brick walls with interlocking corners, recessed mortar
 * - Harsh fluorescent lighting with strong shadows
 * - Dirt, wear, and imperfections throughout
 * - Proper sign placement on back wall near door
 * - Worn concrete floor with stains
 *
 * Controls:
 *   W/S/A/D - Move camera
 *   Q/E - Move up/down
 *   Mouse drag - Look around
 *   ESC - Exit
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <random>

const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;

// Camera - centered view showing all three walls
glm::vec3 cameraPos = glm::vec3(0.0f, 1.5f, 3.2f);  // Centered, eye level, pulled back
glm::vec3 cameraFront = glm::vec3(0.0f, -0.05f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float yaw = -90.0f;  // Looking straight ahead
float pitch = -3.0f;
float lastX = WINDOW_WIDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;
bool mousePressed = false;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void processInput(GLFWwindow* window);
GLuint loadShaders(const char* vertexPath, const char* fragmentPath);
std::string readFile(const char* filePath);

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 normal;
};

struct SceneObject {
    GLuint VAO, VBO;
    int vertexCount;
    glm::mat4 modelMatrix;
    std::string name;
};

std::vector<SceneObject> sceneObjects;

// Random number generator for consistent imperfections
std::mt19937 globalRng(12345);

float randomFloat(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(globalRng);
}

/**
 * Apply dirt/wear gradient - darker near floor, in corners
 */
glm::vec3 applyWear(glm::vec3 baseColor, float y, float distFromCorner, float distFromFloor) {
    float dirtFactor = 1.0f;

    // Dirt accumulation near floor (0-30cm)
    if (y < 0.3f) {
        dirtFactor *= 0.7f + 0.3f * (y / 0.3f);
    }

    // Extra dirt in corners
    if (distFromCorner < 0.5f) {
        dirtFactor *= 0.85f + 0.15f * (distFromCorner / 0.5f);
    }

    // Random wear spots
    float wear = randomFloat(0.95f, 1.02f);

    return baseColor * dirtFactor * wear;
}

std::vector<Vertex> createQuad(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, glm::vec3 color) {
    glm::vec3 edge1 = p2 - p1;
    glm::vec3 edge2 = p4 - p1;
    glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

    std::vector<Vertex> vertices = {
        {p1, color, normal}, {p2, color, normal}, {p3, color, normal},
        {p1, color, normal}, {p3, color, normal}, {p4, color, normal}
    };
    return vertices;
}

/**
 * Create realistic CMU (Concrete Masonry Unit) wall matching real photo:
 * - Large CMU blocks (~8"x16" / 0.20m x 0.40m)
 * - Uniform terracotta/salmon color with subtle variation
 * - Dark recessed mortar lines
 * - Running bond pattern
 */
std::vector<Vertex> createRealisticBrickWall(glm::vec3 origin, glm::vec3 uDir, glm::vec3 vDir,
                                              float width, float height, glm::vec3 baseColor,
                                              bool isCornerLeft, bool isCornerRight) {
    std::vector<Vertex> vertices;

    // CMU block dimensions - larger to match real photo
    float brickWidth = 0.45f;    // Wider blocks as seen in photo
    float brickHeight = 0.22f;   // Taller blocks
    float mortarThick = 0.022f;  // Visible mortar joint
    float mortarRecess = 0.010f; // Recess for shadow
    float shadowLineThick = 0.004f; // Dark shadow line thickness

    glm::vec3 normal = glm::normalize(glm::cross(uDir, vDir));

    // Mortar color - light beige/cream cement (matching reference)
    glm::vec3 mortarBase(0.82f, 0.78f, 0.70f);
    // Shadow line color - dark for depth
    glm::vec3 shadowColor(0.35f, 0.32f, 0.28f);

    std::mt19937 brickRng(42 + (int)(origin.x * 100) + (int)(origin.z * 100));
    std::uniform_real_distribution<float> colorNoise(-0.03f, 0.03f);  // Subtle variation
    std::uniform_real_distribution<float> alignNoise(-0.001f, 0.001f);
    std::uniform_real_distribution<float> mortarNoise(-0.05f, 0.05f);  // Grain variation
    std::uniform_real_distribution<float> grainNoise(-0.04f, 0.04f);   // Extra grain texture

    int rows = (int)(height / (brickHeight + mortarThick)) + 1;

    for (int row = 0; row < rows; row++) {
        float rowOffset = (row % 2 == 0) ? 0.0f : brickWidth / 2.0f;

        // Corner interlocking - alternate header bricks at corners
        if (isCornerLeft && row % 2 == 1) {
            rowOffset = -brickWidth / 4.0f;
        }
        if (isCornerRight && row % 2 == 1) {
            rowOffset = brickWidth / 4.0f;
        }

        float v = row * (brickHeight + mortarThick);
        if (v >= height) continue;

        // Horizontal mortar line with shadow lines and grainy cream center
        if (row > 0) {
            float mortarV = v - mortarThick;
            glm::vec3 recessOffset = normal * (-mortarRecess);
            glm::vec3 deepRecess = normal * (-mortarRecess * 1.5f);

            // Create multiple small segments for grainy texture
            int mortarSegments = (int)(width / 0.04f) + 1;
            float segmentWidth = width / mortarSegments;

            for (int seg = 0; seg < mortarSegments; seg++) {
                float segStart = seg * segmentWidth;
                float segEnd = std::min((seg + 1) * segmentWidth, width);

                // Bottom shadow line (dark, recessed deeper)
                glm::vec3 shadowVar = shadowColor + glm::vec3(grainNoise(brickRng) * 0.5f);
                glm::vec3 s1 = origin + uDir * segStart + vDir * mortarV + deepRecess;
                glm::vec3 s2 = origin + uDir * segEnd + vDir * mortarV + deepRecess;
                glm::vec3 s3 = origin + uDir * segEnd + vDir * (mortarV + shadowLineThick) + deepRecess;
                glm::vec3 s4 = origin + uDir * segStart + vDir * (mortarV + shadowLineThick) + deepRecess;
                vertices.push_back({s1, shadowVar, normal});
                vertices.push_back({s2, shadowVar, normal});
                vertices.push_back({s3, shadowVar, normal});
                vertices.push_back({s1, shadowVar, normal});
                vertices.push_back({s3, shadowVar, normal});
                vertices.push_back({s4, shadowVar, normal});

                // Main cream mortar (grainy)
                glm::vec3 mortarColor = mortarBase + glm::vec3(
                    mortarNoise(brickRng) + grainNoise(brickRng),
                    mortarNoise(brickRng) + grainNoise(brickRng) * 0.9f,
                    mortarNoise(brickRng) + grainNoise(brickRng) * 0.7f
                );
                mortarColor = glm::clamp(mortarColor, glm::vec3(0.65f), glm::vec3(0.92f));

                glm::vec3 m1 = origin + uDir * segStart + vDir * (mortarV + shadowLineThick) + recessOffset;
                glm::vec3 m2 = origin + uDir * segEnd + vDir * (mortarV + shadowLineThick) + recessOffset;
                glm::vec3 m3 = origin + uDir * segEnd + vDir * (mortarV + mortarThick - shadowLineThick) + recessOffset;
                glm::vec3 m4 = origin + uDir * segStart + vDir * (mortarV + mortarThick - shadowLineThick) + recessOffset;
                vertices.push_back({m1, mortarColor, normal});
                vertices.push_back({m2, mortarColor, normal});
                vertices.push_back({m3, mortarColor, normal});
                vertices.push_back({m1, mortarColor, normal});
                vertices.push_back({m3, mortarColor, normal});
                vertices.push_back({m4, mortarColor, normal});

                // Top shadow line (dark, recessed deeper)
                shadowVar = shadowColor + glm::vec3(grainNoise(brickRng) * 0.5f);
                glm::vec3 t1 = origin + uDir * segStart + vDir * (mortarV + mortarThick - shadowLineThick) + deepRecess;
                glm::vec3 t2 = origin + uDir * segEnd + vDir * (mortarV + mortarThick - shadowLineThick) + deepRecess;
                glm::vec3 t3 = origin + uDir * segEnd + vDir * (mortarV + mortarThick) + deepRecess;
                glm::vec3 t4 = origin + uDir * segStart + vDir * (mortarV + mortarThick) + deepRecess;
                vertices.push_back({t1, shadowVar, normal});
                vertices.push_back({t2, shadowVar, normal});
                vertices.push_back({t3, shadowVar, normal});
                vertices.push_back({t1, shadowVar, normal});
                vertices.push_back({t3, shadowVar, normal});
                vertices.push_back({t4, shadowVar, normal});
            }
        }

        int cols = (int)(width / (brickWidth + mortarThick)) + 2;

        for (int col = -1; col < cols; col++) {
            float u = col * (brickWidth + mortarThick) - rowOffset;

            // Imperfect alignment
            u += alignNoise(brickRng);
            float vOffset = alignNoise(brickRng);

            float brickStart = std::max(0.0f, u);
            float brickEnd = std::min(width, u + brickWidth);

            if (brickEnd <= brickStart) continue;
            if (v + brickHeight > height) continue;

            // Per-block color variation - subtle for uniform CMU look
            float rNoise = colorNoise(brickRng);
            float gNoise = colorNoise(brickRng) * 0.8f;
            float bNoise = colorNoise(brickRng) * 0.6f;

            // Occasional slightly darker or lighter block (subtle)
            float brickVariant = randomFloat(0.0f, 1.0f);
            if (brickVariant < 0.05f) {
                rNoise -= 0.02f; gNoise -= 0.015f; bNoise -= 0.01f;
            } else if (brickVariant > 0.95f) {
                rNoise += 0.02f; gNoise += 0.015f;
            }

            glm::vec3 brickColor = baseColor + glm::vec3(rNoise, gNoise, bNoise);

            // Apply dirt near floor
            float distFromFloor = v;
            brickColor = applyWear(brickColor, distFromFloor, 1.0f, distFromFloor);
            brickColor = glm::clamp(brickColor, glm::vec3(0.0f), glm::vec3(1.0f));

            glm::vec3 p1 = origin + uDir * brickStart + vDir * (v + vOffset);
            glm::vec3 p2 = origin + uDir * brickEnd + vDir * (v + vOffset);
            glm::vec3 p3 = origin + uDir * brickEnd + vDir * (v + brickHeight + vOffset);
            glm::vec3 p4 = origin + uDir * brickStart + vDir * (v + brickHeight + vOffset);

            vertices.push_back({p1, brickColor, normal});
            vertices.push_back({p2, brickColor, normal});
            vertices.push_back({p3, brickColor, normal});
            vertices.push_back({p1, brickColor, normal});
            vertices.push_back({p3, brickColor, normal});
            vertices.push_back({p4, brickColor, normal});

            // Vertical mortar with shadow lines and grainy cream center
            if (u > 0 && u < width - brickWidth) {
                glm::vec3 recessOffset = normal * (-mortarRecess);
                glm::vec3 deepRecess = normal * (-mortarRecess * 1.5f);
                float mortarU = brickStart - mortarThick;

                if (mortarU >= 0) {
                    // Create multiple small segments for grainy texture
                    int mortarSegments = (int)(brickHeight / 0.035f) + 1;
                    float segmentHeight = brickHeight / mortarSegments;

                    for (int seg = 0; seg < mortarSegments; seg++) {
                        float segStart = v + seg * segmentHeight;
                        float segEnd = v + std::min((seg + 1) * segmentHeight, brickHeight);

                        // Left shadow line
                        glm::vec3 shadowVar = shadowColor + glm::vec3(grainNoise(brickRng) * 0.5f);
                        glm::vec3 sl1 = origin + uDir * mortarU + vDir * segStart + deepRecess;
                        glm::vec3 sl2 = origin + uDir * (mortarU + shadowLineThick) + vDir * segStart + deepRecess;
                        glm::vec3 sl3 = origin + uDir * (mortarU + shadowLineThick) + vDir * segEnd + deepRecess;
                        glm::vec3 sl4 = origin + uDir * mortarU + vDir * segEnd + deepRecess;
                        vertices.push_back({sl1, shadowVar, normal});
                        vertices.push_back({sl2, shadowVar, normal});
                        vertices.push_back({sl3, shadowVar, normal});
                        vertices.push_back({sl1, shadowVar, normal});
                        vertices.push_back({sl3, shadowVar, normal});
                        vertices.push_back({sl4, shadowVar, normal});

                        // Main cream mortar (grainy)
                        glm::vec3 mortarColor = mortarBase + glm::vec3(
                            mortarNoise(brickRng) + grainNoise(brickRng),
                            mortarNoise(brickRng) + grainNoise(brickRng) * 0.9f,
                            mortarNoise(brickRng) + grainNoise(brickRng) * 0.7f
                        );
                        mortarColor = glm::clamp(mortarColor, glm::vec3(0.65f), glm::vec3(0.92f));

                        glm::vec3 vm1 = origin + uDir * (mortarU + shadowLineThick) + vDir * segStart + recessOffset;
                        glm::vec3 vm2 = origin + uDir * (brickStart - shadowLineThick) + vDir * segStart + recessOffset;
                        glm::vec3 vm3 = origin + uDir * (brickStart - shadowLineThick) + vDir * segEnd + recessOffset;
                        glm::vec3 vm4 = origin + uDir * (mortarU + shadowLineThick) + vDir * segEnd + recessOffset;
                        vertices.push_back({vm1, mortarColor, normal});
                        vertices.push_back({vm2, mortarColor, normal});
                        vertices.push_back({vm3, mortarColor, normal});
                        vertices.push_back({vm1, mortarColor, normal});
                        vertices.push_back({vm3, mortarColor, normal});
                        vertices.push_back({vm4, mortarColor, normal});

                        // Right shadow line
                        shadowVar = shadowColor + glm::vec3(grainNoise(brickRng) * 0.5f);
                        glm::vec3 sr1 = origin + uDir * (brickStart - shadowLineThick) + vDir * segStart + deepRecess;
                        glm::vec3 sr2 = origin + uDir * brickStart + vDir * segStart + deepRecess;
                        glm::vec3 sr3 = origin + uDir * brickStart + vDir * segEnd + deepRecess;
                        glm::vec3 sr4 = origin + uDir * (brickStart - shadowLineThick) + vDir * segEnd + deepRecess;
                        vertices.push_back({sr1, shadowVar, normal});
                        vertices.push_back({sr2, shadowVar, normal});
                        vertices.push_back({sr3, shadowVar, normal});
                        vertices.push_back({sr1, shadowVar, normal});
                        vertices.push_back({sr3, shadowVar, normal});
                        vertices.push_back({sr4, shadowVar, normal});
                    }
                }
            }
        }
    }

    return vertices;
}

std::vector<Vertex> createBox(glm::vec3 center, glm::vec3 size, glm::vec3 color) {
    float hx = size.x / 2.0f;
    float hy = size.y / 2.0f;
    float hz = size.z / 2.0f;

    std::vector<Vertex> vertices;

    // Front (+Z)
    glm::vec3 fn(0, 0, 1);
    vertices.push_back({{center.x - hx, center.y - hy, center.z + hz}, color, fn});
    vertices.push_back({{center.x + hx, center.y - hy, center.z + hz}, color, fn});
    vertices.push_back({{center.x + hx, center.y + hy, center.z + hz}, color, fn});
    vertices.push_back({{center.x - hx, center.y - hy, center.z + hz}, color, fn});
    vertices.push_back({{center.x + hx, center.y + hy, center.z + hz}, color, fn});
    vertices.push_back({{center.x - hx, center.y + hy, center.z + hz}, color, fn});

    // Back (-Z)
    glm::vec3 bn(0, 0, -1);
    vertices.push_back({{center.x + hx, center.y - hy, center.z - hz}, color, bn});
    vertices.push_back({{center.x - hx, center.y - hy, center.z - hz}, color, bn});
    vertices.push_back({{center.x - hx, center.y + hy, center.z - hz}, color, bn});
    vertices.push_back({{center.x + hx, center.y - hy, center.z - hz}, color, bn});
    vertices.push_back({{center.x - hx, center.y + hy, center.z - hz}, color, bn});
    vertices.push_back({{center.x + hx, center.y + hy, center.z - hz}, color, bn});

    // Right (+X)
    glm::vec3 rn(1, 0, 0);
    vertices.push_back({{center.x + hx, center.y - hy, center.z + hz}, color, rn});
    vertices.push_back({{center.x + hx, center.y - hy, center.z - hz}, color, rn});
    vertices.push_back({{center.x + hx, center.y + hy, center.z - hz}, color, rn});
    vertices.push_back({{center.x + hx, center.y - hy, center.z + hz}, color, rn});
    vertices.push_back({{center.x + hx, center.y + hy, center.z - hz}, color, rn});
    vertices.push_back({{center.x + hx, center.y + hy, center.z + hz}, color, rn});

    // Left (-X)
    glm::vec3 ln(-1, 0, 0);
    vertices.push_back({{center.x - hx, center.y - hy, center.z - hz}, color, ln});
    vertices.push_back({{center.x - hx, center.y - hy, center.z + hz}, color, ln});
    vertices.push_back({{center.x - hx, center.y + hy, center.z + hz}, color, ln});
    vertices.push_back({{center.x - hx, center.y - hy, center.z - hz}, color, ln});
    vertices.push_back({{center.x - hx, center.y + hy, center.z + hz}, color, ln});
    vertices.push_back({{center.x - hx, center.y + hy, center.z - hz}, color, ln});

    // Top (+Y)
    glm::vec3 tn(0, 1, 0);
    vertices.push_back({{center.x - hx, center.y + hy, center.z + hz}, color, tn});
    vertices.push_back({{center.x + hx, center.y + hy, center.z + hz}, color, tn});
    vertices.push_back({{center.x + hx, center.y + hy, center.z - hz}, color, tn});
    vertices.push_back({{center.x - hx, center.y + hy, center.z + hz}, color, tn});
    vertices.push_back({{center.x + hx, center.y + hy, center.z - hz}, color, tn});
    vertices.push_back({{center.x - hx, center.y + hy, center.z - hz}, color, tn});

    // Bottom (-Y)
    glm::vec3 botn(0, -1, 0);
    vertices.push_back({{center.x - hx, center.y - hy, center.z - hz}, color, botn});
    vertices.push_back({{center.x + hx, center.y - hy, center.z - hz}, color, botn});
    vertices.push_back({{center.x + hx, center.y - hy, center.z + hz}, color, botn});
    vertices.push_back({{center.x - hx, center.y - hy, center.z - hz}, color, botn});
    vertices.push_back({{center.x + hx, center.y - hy, center.z + hz}, color, botn});
    vertices.push_back({{center.x - hx, center.y - hy, center.z + hz}, color, botn});

    return vertices;
}

// Box with worn edges (lighter color on edges to simulate paint wear)
std::vector<Vertex> createWornBox(glm::vec3 center, glm::vec3 size, glm::vec3 color, glm::vec3 wornColor) {
    auto verts = createBox(center, size, color);

    // Add edge highlights (thin boxes on edges)
    float edgeThick = 0.002f;

    // Top front edge
    auto edge1 = createBox(
        glm::vec3(center.x, center.y + size.y/2, center.z + size.z/2),
        glm::vec3(size.x, edgeThick, edgeThick),
        wornColor
    );
    verts.insert(verts.end(), edge1.begin(), edge1.end());

    return verts;
}

// Door recess with ambient occlusion coloring
std::vector<Vertex> createDoorRecess(glm::vec3 center, float width, float height, float depth, glm::vec3 wallColor) {
    std::vector<Vertex> vertices;

    // Darker color for recessed areas (AO simulation)
    glm::vec3 recessColor = wallColor * 0.7f;
    glm::vec3 deepRecessColor = wallColor * 0.55f;

    float hx = width / 2.0f;
    float hy = height / 2.0f;

    // Top of recess (looking up into it - darkest)
    glm::vec3 topNorm(0, -1, 0);
    vertices.push_back({{center.x - hx, center.y + hy, 0}, deepRecessColor, topNorm});
    vertices.push_back({{center.x + hx, center.y + hy, 0}, deepRecessColor, topNorm});
    vertices.push_back({{center.x + hx, center.y + hy, depth}, recessColor, topNorm});
    vertices.push_back({{center.x - hx, center.y + hy, 0}, deepRecessColor, topNorm});
    vertices.push_back({{center.x + hx, center.y + hy, depth}, recessColor, topNorm});
    vertices.push_back({{center.x - hx, center.y + hy, depth}, recessColor, topNorm});

    // Left side of recess
    glm::vec3 leftNorm(1, 0, 0);
    vertices.push_back({{center.x - hx, center.y - hy, 0}, recessColor, leftNorm});
    vertices.push_back({{center.x - hx, center.y - hy, depth}, recessColor, leftNorm});
    vertices.push_back({{center.x - hx, center.y + hy, depth}, deepRecessColor, leftNorm});
    vertices.push_back({{center.x - hx, center.y - hy, 0}, recessColor, leftNorm});
    vertices.push_back({{center.x - hx, center.y + hy, depth}, deepRecessColor, leftNorm});
    vertices.push_back({{center.x - hx, center.y + hy, 0}, deepRecessColor, leftNorm});

    // Right side of recess
    glm::vec3 rightNorm(-1, 0, 0);
    vertices.push_back({{center.x + hx, center.y - hy, depth}, recessColor, rightNorm});
    vertices.push_back({{center.x + hx, center.y - hy, 0}, recessColor, rightNorm});
    vertices.push_back({{center.x + hx, center.y + hy, 0}, deepRecessColor, rightNorm});
    vertices.push_back({{center.x + hx, center.y - hy, depth}, recessColor, rightNorm});
    vertices.push_back({{center.x + hx, center.y + hy, 0}, deepRecessColor, rightNorm});
    vertices.push_back({{center.x + hx, center.y + hy, depth}, deepRecessColor, rightNorm});

    return vertices;
}

SceneObject createSceneObject(const std::vector<Vertex>& vertices, const std::string& name) {
    SceneObject obj;
    obj.name = name;
    obj.vertexCount = vertices.size();
    obj.modelMatrix = glm::mat4(1.0f);

    glGenVertexArrays(1, &obj.VAO);
    glGenBuffers(1, &obj.VBO);

    glBindVertexArray(obj.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, obj.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    return obj;
}

void initializeScene() {
    // ============================================================
    // COLORS - Precisely matching real scene
    // ============================================================
    glm::vec3 brickColor(0.76f, 0.52f, 0.44f);        // Salmon/terracotta CMU (warmer)
    glm::vec3 floorColor(0.78f, 0.76f, 0.73f);        // Light beige-gray concrete
    glm::vec3 ceilingColor(0.90f, 0.88f, 0.85f);      // Light cream ceiling
    glm::vec3 doorColor(0.45f, 0.47f, 0.49f);         // Medium gray metal door
    glm::vec3 doorWornColor(0.50f, 0.52f, 0.54f);     // Slightly lighter worn edges
    glm::vec3 doorFrameColor(0.38f, 0.40f, 0.42f);    // Steel frame
    glm::vec3 glassColor(0.55f, 0.60f, 0.58f);        // Wired safety glass
    glm::vec3 handleColor(0.75f, 0.75f, 0.73f);       // Chrome hardware
    glm::vec3 signRed(0.78f, 0.15f, 0.15f);           // Safety red
    glm::vec3 signWhite(0.95f, 0.93f, 0.90f);         // White text
    glm::vec3 lightColor(0.98f, 0.98f, 1.0f);         // Bright fluorescent white
    glm::vec3 fixtureColor(0.92f, 0.92f, 0.90f);      // White light fixture

    float roomWidth = 3.5f;
    float roomHeight = 2.75f;
    float roomDepth = 3.5f;

    // ============================================================
    // BRICK WALLS - With interlocking corners
    // ============================================================

    // Back wall
    auto backWallVerts = createRealisticBrickWall(
        glm::vec3(-roomWidth/2, 0.0f, 0.0f),
        glm::vec3(1, 0, 0), glm::vec3(0, 1, 0),
        roomWidth, roomHeight, brickColor, true, true
    );
    sceneObjects.push_back(createSceneObject(backWallVerts, "Back Wall"));

    // Left wall - interlocks with back wall
    auto leftWallVerts = createRealisticBrickWall(
        glm::vec3(-roomWidth/2, 0.0f, roomDepth),
        glm::vec3(0, 0, -1), glm::vec3(0, 1, 0),
        roomDepth, roomHeight, brickColor, false, true
    );
    sceneObjects.push_back(createSceneObject(leftWallVerts, "Left Wall"));

    // Right wall
    auto rightWallVerts = createRealisticBrickWall(
        glm::vec3(roomWidth/2, 0.0f, 0.0f),
        glm::vec3(0, 0, 1), glm::vec3(0, 1, 0),
        roomDepth, roomHeight, brickColor, true, false
    );
    sceneObjects.push_back(createSceneObject(rightWallVerts, "Right Wall"));

    // ============================================================
    // FLOOR - Light gray concrete (matching real photo)
    // ============================================================
    std::vector<Vertex> floorVerts;
    glm::vec3 floorNorm(0, 1, 0);

    std::mt19937 floorRng(999);
    std::uniform_real_distribution<float> stainDist(0.0f, 1.0f);
    std::uniform_real_distribution<float> colorDist(-0.015f, 0.015f);

    // Smaller tiles for smoother concrete look
    int tilesX = 16;
    int tilesZ = 16;
    float tileW = roomWidth / tilesX;
    float tileD = roomDepth / tilesZ;

    for (int i = 0; i < tilesX; i++) {
        for (int j = 0; j < tilesZ; j++) {
            float x1 = -roomWidth/2 + i * tileW;
            float z1 = j * tileD;
            float x2 = x1 + tileW;
            float z2 = z1 + tileD;

            glm::vec3 tileColor = floorColor + glm::vec3(colorDist(floorRng));

            // Subtle darkening near walls (dirt accumulation)
            float distFromWall = std::min({x1 + roomWidth/2, roomWidth/2 - x2, z1});
            if (distFromWall < 0.3f) {
                tileColor *= 0.92f;
            }

            // Very occasional subtle stain
            if (stainDist(floorRng) < 0.05f) {
                tileColor *= 0.95f;
            }

            floorVerts.push_back({{x1, 0, z2}, tileColor, floorNorm});
            floorVerts.push_back({{x2, 0, z2}, tileColor, floorNorm});
            floorVerts.push_back({{x2, 0, z1}, tileColor, floorNorm});
            floorVerts.push_back({{x1, 0, z2}, tileColor, floorNorm});
            floorVerts.push_back({{x2, 0, z1}, tileColor, floorNorm});
            floorVerts.push_back({{x1, 0, z1}, tileColor, floorNorm});
        }
    }

    sceneObjects.push_back(createSceneObject(floorVerts, "Floor"));

    // ============================================================
    // CEILING - Darker, institutional
    // ============================================================
    auto ceilingVerts = createQuad(
        glm::vec3(-roomWidth/2, roomHeight, 0.0f),
        glm::vec3(roomWidth/2, roomHeight, 0.0f),
        glm::vec3(roomWidth/2, roomHeight, roomDepth),
        glm::vec3(-roomWidth/2, roomHeight, roomDepth),
        ceilingColor
    );
    sceneObjects.push_back(createSceneObject(ceilingVerts, "Ceiling"));

    // ============================================================
    // DOOR - Anatomically correct industrial door
    // Handle on LEFT (push side), window OFFSET to hinge side
    // ============================================================
    float doorWidth = 0.91f;   // Standard 36" door
    float doorHeight = 2.10f;  // Standard 7' door
    float doorThick = 0.044f;  // 1.75" hollow metal door
    float doorX = 0.75f;       // Position on back wall - more to the right
    float insetDepth = 0.08f;  // Door recessed into wall

    // Door recess with AO
    auto recessVerts = createDoorRecess(
        glm::vec3(doorX, doorHeight/2, 0),
        doorWidth + 0.15f, doorHeight + 0.08f, insetDepth,
        brickColor
    );
    sceneObjects.push_back(createSceneObject(recessVerts, "Door Recess"));

    // Door body - painted metal
    float doorZ = insetDepth - doorThick/2 - 0.01f;
    auto doorVerts = createWornBox(
        glm::vec3(doorX, doorHeight/2, doorZ),
        glm::vec3(doorWidth, doorHeight, doorThick),
        doorColor, doorWornColor
    );
    sceneObjects.push_back(createSceneObject(doorVerts, "Door"));

    // Door kickplate (bottom) - more worn
    auto kickplateVerts = createBox(
        glm::vec3(doorX, 0.15f, doorZ + doorThick/2 + 0.002f),
        glm::vec3(doorWidth - 0.04f, 0.25f, 0.003f),
        doorColor * 0.85f
    );
    sceneObjects.push_back(createSceneObject(kickplateVerts, "Kickplate"));

    // ============================================================
    // DOOR FRAME - Heavy gauge steel, worn
    // ============================================================
    float frameWidth = 0.05f;
    float frameDepth = insetDepth + 0.03f;

    // Left frame (hinge side)
    auto leftFrameVerts = createBox(
        glm::vec3(doorX - doorWidth/2 - frameWidth/2, doorHeight/2, frameDepth/2),
        glm::vec3(frameWidth, doorHeight + 0.06f, frameDepth),
        doorFrameColor
    );
    sceneObjects.push_back(createSceneObject(leftFrameVerts, "Frame Left"));

    // Right frame (latch side)
    auto rightFrameVerts = createBox(
        glm::vec3(doorX + doorWidth/2 + frameWidth/2, doorHeight/2, frameDepth/2),
        glm::vec3(frameWidth, doorHeight + 0.06f, frameDepth),
        doorFrameColor
    );
    sceneObjects.push_back(createSceneObject(rightFrameVerts, "Frame Right"));

    // Top frame
    auto topFrameVerts = createBox(
        glm::vec3(doorX, doorHeight + frameWidth/2, frameDepth/2),
        glm::vec3(doorWidth + frameWidth * 2, frameWidth, frameDepth),
        doorFrameColor
    );
    sceneObjects.push_back(createSceneObject(topFrameVerts, "Frame Top"));

    // ============================================================
    // DOOR WINDOW - Narrow vertical window with wire mesh glass (matching real photo)
    // ============================================================
    float windowWidth = 0.08f;   // Narrow window
    float windowHeight = 0.50f;  // Taller window
    float windowY = doorHeight * 0.62f;  // Positioned higher
    float windowX = doorX;  // Centered on door
    float windowZ = doorZ + doorThick/2;

    // Window frame (steel) - dark gray
    auto windowFrameVerts = createBox(
        glm::vec3(windowX, windowY, windowZ + 0.006f),
        glm::vec3(windowWidth + 0.03f, windowHeight + 0.03f, 0.015f),
        doorFrameColor
    );
    sceneObjects.push_back(createSceneObject(windowFrameVerts, "Window Frame"));

    // Glass background - wired safety glass (slightly greenish-gray)
    auto glassVerts = createBox(
        glm::vec3(windowX, windowY, windowZ + 0.012f),
        glm::vec3(windowWidth, windowHeight, 0.004f),
        glm::vec3(0.65f, 0.70f, 0.68f)  // Light greenish glass
    );
    sceneObjects.push_back(createSceneObject(glassVerts, "Window Glass"));

    // Wire mesh pattern inside glass (horizontal wires)
    glm::vec3 wireColor(0.45f, 0.45f, 0.45f);
    float wireSpacing = 0.025f;
    int numHorizWires = (int)(windowHeight / wireSpacing);
    for (int i = 1; i < numHorizWires; i++) {
        float wireY = windowY - windowHeight/2 + i * wireSpacing;
        auto wireVerts = createBox(
            glm::vec3(windowX, wireY, windowZ + 0.014f),
            glm::vec3(windowWidth - 0.005f, 0.002f, 0.001f),
            wireColor
        );
        sceneObjects.push_back(createSceneObject(wireVerts, "Wire Horiz"));
    }

    // Vertical wires
    int numVertWires = (int)(windowWidth / wireSpacing);
    for (int i = 1; i < numVertWires + 1; i++) {
        float wireX = windowX - windowWidth/2 + i * wireSpacing;
        auto wireVerts = createBox(
            glm::vec3(wireX, windowY, windowZ + 0.014f),
            glm::vec3(0.002f, windowHeight - 0.005f, 0.001f),
            wireColor
        );
        sceneObjects.push_back(createSceneObject(wireVerts, "Wire Vert"));
    }

    // Glass highlight (reflection simulation - subtle)
    auto glassHighlightVerts = createBox(
        glm::vec3(windowX - 0.015f, windowY + 0.12f, windowZ + 0.016f),
        glm::vec3(0.02f, 0.18f, 0.001f),
        glm::vec3(0.80f, 0.82f, 0.85f)
    );
    sceneObjects.push_back(createSceneObject(glassHighlightVerts, "Glass Highlight"));

    // ============================================================
    // DOOR HARDWARE - Handle on RIGHT (push bar side), hinges on LEFT
    // ============================================================

    // Handle side is RIGHT when looking at door
    float handleX = doorX + doorWidth/2 - 0.10f;
    float handleY = 1.02f;  // Standard 40" AFF

    // Push bar backplate
    auto handlePlateVerts = createBox(
        glm::vec3(handleX, handleY, windowZ + 0.015f),
        glm::vec3(0.06f, 0.18f, 0.008f),
        handleColor * 0.9f
    );
    sceneObjects.push_back(createSceneObject(handlePlateVerts, "Handle Plate"));

    // Push bar lever
    auto handleVerts = createBox(
        glm::vec3(handleX - 0.02f, handleY, windowZ + 0.035f),
        glm::vec3(0.10f, 0.022f, 0.022f),
        handleColor
    );
    sceneObjects.push_back(createSceneObject(handleVerts, "Handle"));

    // Hinges on LEFT side (3 hinges for commercial door)
    float hingeX = doorX - doorWidth/2 + 0.05f;
    float hingeZ = doorZ - doorThick/2;
    for (float hingeY : {0.25f, 1.07f, 1.90f}) {
        auto hingeVerts = createBox(
            glm::vec3(hingeX, hingeY, hingeZ - 0.01f),
            glm::vec3(0.03f, 0.10f, 0.02f),
            handleColor * 0.85f
        );
        sceneObjects.push_back(createSceneObject(hingeVerts, "Hinge"));
    }

    // Door closer (prominently visible at top right of door, matching real photo)
    float closerY = doorHeight - 0.06f;
    float closerX = doorX + doorWidth/2 - 0.15f;

    // Closer body (cylinder-like box)
    auto closerBodyVerts = createBox(
        glm::vec3(closerX, closerY, windowZ + 0.04f),
        glm::vec3(0.22f, 0.05f, 0.055f),
        glm::vec3(0.55f, 0.55f, 0.55f)  // Silver/chrome
    );
    sceneObjects.push_back(createSceneObject(closerBodyVerts, "Door Closer"));

    // Closer arm (extends from closer to frame)
    auto closerArmVerts = createBox(
        glm::vec3(closerX + 0.15f, closerY + 0.01f, windowZ + 0.05f),
        glm::vec3(0.12f, 0.018f, 0.018f),
        glm::vec3(0.60f, 0.60f, 0.60f)
    );
    sceneObjects.push_back(createSceneObject(closerArmVerts, "Closer Arm"));

    // Arm bracket on frame
    auto closerBracketVerts = createBox(
        glm::vec3(doorX + doorWidth/2 + 0.04f, closerY + 0.01f, windowZ + 0.03f),
        glm::vec3(0.03f, 0.04f, 0.025f),
        glm::vec3(0.55f, 0.55f, 0.55f)
    );
    sceneObjects.push_back(createSceneObject(closerBracketVerts, "Closer Bracket"));

    // ============================================================
    // WALL-MOUNTED LIGHT FIXTURE - Above door on back wall (matching real photo)
    // ============================================================
    float lightX = 0.0f;   // Centered on wall (slightly left of door)
    float lightY = 2.40f;  // High on wall, below ceiling
    float lightZ = 0.06f;  // Mounted on back wall

    // Fixture housing (wall-mounted fluorescent tube - horizontal bar)
    auto housingVerts = createBox(
        glm::vec3(lightX, lightY, lightZ + 0.04f),
        glm::vec3(1.1f, 0.08f, 0.10f),
        fixtureColor
    );
    sceneObjects.push_back(createSceneObject(housingVerts, "Light Housing"));

    // Diffuser lens (the glowing part - bottom face)
    auto diffuserVerts = createBox(
        glm::vec3(lightX, lightY - 0.035f, lightZ + 0.06f),
        glm::vec3(1.0f, 0.015f, 0.07f),
        lightColor
    );
    sceneObjects.push_back(createSceneObject(diffuserVerts, "Light Diffuser"));

    // Front face of light (also glowing)
    auto diffuserFrontVerts = createBox(
        glm::vec3(lightX, lightY, lightZ + 0.10f),
        glm::vec3(1.0f, 0.055f, 0.01f),
        lightColor
    );
    sceneObjects.push_back(createSceneObject(diffuserFrontVerts, "Light Diffuser Front"));

    // ============================================================
    // SIGNS - Matching real photo placement exactly
    // ============================================================
    float signZ = 0.02f;  // Mounted proud of wall

    // Emergency Phone Sign - Far left on back wall
    float emergencyX = -0.85f;
    float emergencyY = 1.25f;

    // Sign face (red background)
    auto emergencySignVerts = createBox(
        glm::vec3(emergencyX, emergencyY, signZ + 0.008f),
        glm::vec3(0.26f, 0.18f, 0.012f),
        signRed
    );
    sceneObjects.push_back(createSceneObject(emergencySignVerts, "Emergency Sign"));

    // White text areas (multiple lines)
    auto emergencyTitle = createBox(
        glm::vec3(emergencyX, emergencyY + 0.055f, signZ + 0.014f),
        glm::vec3(0.20f, 0.03f, 0.002f),
        signWhite
    );
    sceneObjects.push_back(createSceneObject(emergencyTitle, "Emergency Title"));

    auto emergencyLine1 = createBox(
        glm::vec3(emergencyX, emergencyY + 0.01f, signZ + 0.014f),
        glm::vec3(0.18f, 0.022f, 0.002f),
        signWhite
    );
    sceneObjects.push_back(createSceneObject(emergencyLine1, "Emergency Line1"));

    auto emergencyLine2 = createBox(
        glm::vec3(emergencyX, emergencyY - 0.025f, signZ + 0.014f),
        glm::vec3(0.16f, 0.022f, 0.002f),
        signWhite
    );
    sceneObjects.push_back(createSceneObject(emergencyLine2, "Emergency Line2"));

    auto emergencyLine3 = createBox(
        glm::vec3(emergencyX, emergencyY - 0.058f, signZ + 0.014f),
        glm::vec3(0.14f, 0.022f, 0.002f),
        signWhite
    );
    sceneObjects.push_back(createSceneObject(emergencyLine3, "Emergency Line3"));

    // Floor number sign "2" - Next to door on left side
    float stairsX = doorX - doorWidth/2 - 0.18f;
    float stairsY = 1.50f;

    // Main sign body (red)
    auto stairsSignVerts = createBox(
        glm::vec3(stairsX, stairsY, signZ + 0.008f),
        glm::vec3(0.16f, 0.22f, 0.012f),
        signRed
    );
    sceneObjects.push_back(createSceneObject(stairsSignVerts, "Stairs Sign"));

    // "EAST STAIRS" text at top
    auto stairsTextTop = createBox(
        glm::vec3(stairsX, stairsY + 0.075f, signZ + 0.014f),
        glm::vec3(0.12f, 0.022f, 0.002f),
        signWhite
    );
    sceneObjects.push_back(createSceneObject(stairsTextTop, "Stairs Text Top"));

    // "NO ROOF ACCESS" text
    auto stairsTextMid = createBox(
        glm::vec3(stairsX, stairsY + 0.042f, signZ + 0.014f),
        glm::vec3(0.10f, 0.015f, 0.002f),
        signWhite
    );
    sceneObjects.push_back(createSceneObject(stairsTextMid, "Stairs Text Mid"));

    // Large "2" number
    auto numberVerts = createBox(
        glm::vec3(stairsX, stairsY - 0.035f, signZ + 0.014f),
        glm::vec3(0.09f, 0.11f, 0.003f),
        signWhite
    );
    sceneObjects.push_back(createSceneObject(numberVerts, "Floor Number"));

    // ============================================================
    // ELECTRICAL/PHONE BOX - Below emergency sign on back wall
    // ============================================================
    float elecX = -0.85f;  // Same X as emergency sign
    float elecY = 0.95f;   // Below the sign
    float elecZ = 0.03f;   // On back wall

    // Small phone/electrical box
    auto elecBoxVerts = createBox(
        glm::vec3(elecX, elecY, elecZ),
        glm::vec3(0.08f, 0.10f, 0.05f),
        glm::vec3(0.80f, 0.80f, 0.78f)  // Light gray box
    );
    sceneObjects.push_back(createSceneObject(elecBoxVerts, "Phone Box"));

    // ============================================================
    // DIRT ACCUMULATION - Subtle dark strips at wall-floor junction
    // ============================================================
    glm::vec3 dirtColor(0.45f, 0.42f, 0.40f);  // Lighter dirt to match scene
    float dirtHeight = 0.02f;

    // Back wall dirt line
    auto backDirtVerts = createQuad(
        glm::vec3(-roomWidth/2, 0, 0.001f),
        glm::vec3(roomWidth/2, 0, 0.001f),
        glm::vec3(roomWidth/2, dirtHeight, 0.001f),
        glm::vec3(-roomWidth/2, dirtHeight, 0.001f),
        dirtColor
    );
    sceneObjects.push_back(createSceneObject(backDirtVerts, "Back Wall Dirt"));

    // Left wall dirt line
    auto leftDirtVerts = createQuad(
        glm::vec3(-roomWidth/2 + 0.001f, 0, roomDepth),
        glm::vec3(-roomWidth/2 + 0.001f, 0, 0),
        glm::vec3(-roomWidth/2 + 0.001f, dirtHeight, 0),
        glm::vec3(-roomWidth/2 + 0.001f, dirtHeight, roomDepth),
        dirtColor
    );
    sceneObjects.push_back(createSceneObject(leftDirtVerts, "Left Wall Dirt"));

    std::cout << "Scene initialized with " << sceneObjects.size() << " objects." << std::endl;
}

std::string readFile(const char* filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open file: " << filePath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint loadShaders(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode = readFile(vertexPath);
    std::string fragmentCode = readFile(fragmentPath);

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    GLuint vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cerr << "Vertex shader error: " << infoLog << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cerr << "Fragment shader error: " << infoLog << std::endl;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "Shader link error: " << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (!mousePressed) return;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    xoffset *= 0.1f;
    yoffset *= 0.1f;

    yaw += xoffset;
    pitch += yoffset;

    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        mousePressed = (action == GLFW_PRESS);
        firstMouse = true;
    }
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float speed = 2.0f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += speed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= speed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cameraPos.y -= speed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cameraPos.y += speed;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
        "Stairwell Scene - Realistic", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    GLuint shaderProgram = loadShaders("shaders/vertex.glsl", "shaders/fragment.glsl");

    initializeScene();

    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    GLint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
    GLint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
    GLint ambientLoc = glGetUniformLocation(shaderProgram, "ambientStrength");

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.02f, 0.02f, 0.03f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        // Phone camera field of view (matching real photo perspective)
        glm::mat4 projection = glm::perspective(
            glm::radians(52.0f),
            (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,
            0.05f,
            50.0f
        );

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Wall-mounted fluorescent - positioned on back wall
        glm::vec3 lightPos(0.0f, 2.40f, 0.20f);  // Centered above, on wall
        glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
        glUniform3fv(viewPosLoc, 1, glm::value_ptr(cameraPos));

        // Good ambient for realistic indoor lighting
        glUniform1f(ambientLoc, 0.18f);

        for (const auto& obj : sceneObjects) {
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(obj.modelMatrix));
            glBindVertexArray(obj.VAO);
            glDrawArrays(GL_TRIANGLES, 0, obj.vertexCount);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    for (auto& obj : sceneObjects) {
        glDeleteVertexArrays(1, &obj.VAO);
        glDeleteBuffers(1, &obj.VBO);
    }
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
