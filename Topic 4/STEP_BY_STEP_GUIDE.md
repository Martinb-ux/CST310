# STEP-BY-STEP GUIDE: Building the OpenGL Stairwell Scene

This guide will walk you through the complete process of recreating the stairwell scene in OpenGL from start to finish.

---

## PHASE 1: Environment Setup (30 minutes)

### Step 1: Install Required Software

**Linux (Ubuntu/Debian):**
```bash
# Update package list
sudo apt-get update

# Install build tools
sudo apt-get install build-essential cmake

# Install OpenGL libraries
sudo apt-get install libgl1-mesa-dev

# Install GLFW (window management)
sudo apt-get install libglfw3-dev

# Install GLEW (extension loading)
sudo apt-get install libglew-dev

# Install GLM (mathematics)
sudo apt-get install libglm-dev
```

**macOS:**
```bash
# Install Homebrew if not already installed
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install libraries
brew install glfw glew glm cmake
```

**Windows:**
Download and install:
1. Visual Studio Community 2022 (free)
2. CMake (https://cmake.org/download/)
3. vcpkg package manager:
   ```
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   ./bootstrap-vcpkg.bat
   vcpkg install glfw3 glew glm
   ```

### Step 2: Create Project Directory

```bash
mkdir stairwell_opengl
cd stairwell_opengl
```

### Step 3: Verify Installation

**Linux/macOS:**
```bash
# Check OpenGL
glxinfo | grep "OpenGL version"

# Check libraries
pkg-config --modversion glfw3
pkg-config --modversion glew
```

---

## PHASE 2: Understanding the Architecture (15 minutes)

### The OpenGL Pipeline

```
Vertex Data → Vertex Shader → Rasterization → Fragment Shader → Screen
     ↓              ↓                               ↓
  Positions    Transforms                      Lighting/Color
  Normals      Projects                        Final Color
```

### Scene Decomposition

Look at the reference image and identify these components:
1. **Brick Walls** - Made of ~200 individual brick cubes
2. **Door** - 4 components (body, window, handle, frame)
3. **Signs** - 4 components (2 backgrounds, 2 text areas)
4. **Floor** - Single large flat cube
5. **Ceiling** - Single large flat cube
6. **Light** - 2 components (housing, emitting panel)

### Coordinate System

```
       Y (Up)
       |
       |_____ X (Right)
      /
     Z (Forward/Toward Camera)

Center point: (0, 0, 0)
Camera position: (0, 1.5, 5) - back and at eye level
```

---

## PHASE 3: Create Shader Files (20 minutes)

### Step 1: Create Vertex Shader

Create file: `vertex_shader.glsl`

```glsl
#version 330 core

// Input: vertex position and normal
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

// Output: data for fragment shader
out vec3 FragPos;
out vec3 Normal;

// Transformation matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Transform vertex position to world space
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // Transform normal to world space
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    // Final position in clip space
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
```

**What this does:**
- Takes vertex position and normal as input
- Transforms position through model→view→projection matrices
- Passes world-space position and normal to fragment shader

### Step 2: Create Fragment Shader

Create file: `fragment_shader.glsl`

```glsl
#version 330 core

// Input from vertex shader
in vec3 FragPos;
in vec3 Normal;

// Output: final color
out vec4 FragColor;

// Lighting parameters
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // Ambient lighting (base light)
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;
    
    // Diffuse lighting (directional)
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular lighting (highlights)
    float specularStrength = 0.4;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    
    // Combine all lighting
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
```

**What this does:**
- Implements Phong lighting model
- Calculates ambient, diffuse, and specular components
- Combines lighting with object color

---

## PHASE 4: Write Main Application (90 minutes)

### Step 1: Set Up Includes and Global Variables

Create file: `main.cpp`

```cpp
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

// Window size
const unsigned int SCREEN_WIDTH = 1200;
const unsigned int SCREEN_HEIGHT = 900;

// Camera
glm::vec3 cameraPos = glm::vec3(0.0f, 1.5f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Mouse
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;

// VAO and VBO
unsigned int cubeVAO, cubeVBO;
```

### Step 2: Define Cube Vertices

```cpp
float cubeVertices[] = {
    // positions          // normals
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    // ... (repeat for all 6 faces)
};
```

### Step 3: Create Helper Functions

```cpp
// Load and compile shaders
unsigned int loadShader(const char* vertexPath, const char* fragmentPath);

// Input handling
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Rendering functions
void renderBrickWall(unsigned int shaderProgram);
void renderDoor(unsigned int shaderProgram);
void renderSigns(unsigned int shaderProgram);
void renderFloor(unsigned int shaderProgram);
void renderCeiling(unsigned int shaderProgram);
void renderLight(unsigned int shaderProgram);
```

### Step 4: Write Main Function

```cpp
int main() {
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, 
                                          "Stairwell Scene", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW
    glewInit();
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Load shaders
    unsigned int shaderProgram = loadShader("vertex_shader.glsl", 
                                           "fragment_shader.glsl");

    // Set up vertex data
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), 
                 cubeVertices, GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 
                         (void*)0);
    glEnableVertexAttribArray(0);
    
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 
                         (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        // Timing
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        processInput(window);

        // Clear screen
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use shader
        glUseProgram(shaderProgram);

        // Set up matrices
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 
            (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, 
                                     cameraPos + cameraFront, 
                                     cameraUp);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 
                          1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 
                          1, GL_FALSE, glm::value_ptr(view));

        // Set lighting
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 
                    1, glm::value_ptr(glm::vec3(0.0f, 3.5f, 0.0f)));
        glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 
                    1, glm::value_ptr(cameraPos));
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 
                    1, glm::value_ptr(glm::vec3(1.0f, 0.95f, 0.8f)));

        // Render scene
        renderBrickWall(shaderProgram);
        renderDoor(shaderProgram);
        renderSigns(shaderProgram);
        renderFloor(shaderProgram);
        renderCeiling(shaderProgram);
        renderLight(shaderProgram);

        // Swap and poll
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glfwTerminate();
    return 0;
}
```

### Step 5: Implement Rendering Functions

**Example: Brick Wall**

```cpp
void renderBrickWall(unsigned int shaderProgram) {
    // Set brick color
    glm::vec3 brickColor(0.7f, 0.35f, 0.2f);
    glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 
                1, glm::value_ptr(brickColor));

    // Brick dimensions
    float brickWidth = 0.4f;
    float brickHeight = 0.15f;
    float brickDepth = 0.2f;
    float mortarGap = 0.02f;

    // Render left wall
    for (int row = 0; row < 20; row++) {
        int bricksInRow = 8;
        float offset = (row % 2 == 0) ? 0.0f : brickWidth / 2.0f;
        
        for (int col = 0; col < bricksInRow; col++) {
            glm::mat4 model = glm::mat4(1.0f);
            
            // Calculate position
            float x = -4.0f + offset + col * (brickWidth + mortarGap);
            float y = -1.0f + row * (brickHeight + mortarGap);
            float z = -2.0f;
            
            // Transform
            model = glm::translate(model, glm::vec3(x, y, z));
            model = glm::scale(model, glm::vec3(brickWidth, brickHeight, brickDepth));
            
            // Send to shader and draw
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 
                             1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }
}
```

---

## PHASE 5: Compilation and Testing (20 minutes)

### Method 1: Using CMake (Recommended)

```bash
# Create build directory
mkdir build
cd build

# Generate build files
cmake ..

# Compile
make

# Run
./stairwell_scene
```

### Method 2: Manual Compilation

**Linux:**
```bash
g++ -std=c++11 main.cpp -o stairwell_scene \
    $(pkg-config --cflags --libs glfw3 glew) -lGL -lm
./stairwell_scene
```

**macOS:**
```bash
g++ -std=c++11 main.cpp -o stairwell_scene \
    -lglfw -lGLEW -framework OpenGL -framework Cocoa
./stairwell_scene
```

### Expected Result

You should see:
- Window opens (1200×900)
- 3D scene with brick walls, door, signs, floor, ceiling
- Lighting effects visible
- Can move with WASD, look with mouse
- Press ESC to exit

---

## PHASE 6: Testing and Verification (15 minutes)

### Visual Checklist

✓ Left brick wall visible with running bond pattern
✓ Right brick wall visible at angle
✓ Dark gray door centered
✓ Door window (blue-gray) visible
✓ Door handle (silver) on right side
✓ Door frame above door
✓ Two red signs on left wall
✓ White text areas on signs
✓ Gray floor below
✓ Light gray ceiling above
✓ White light fixture above door
✓ Lighting effects (shadows/highlights)

### Functionality Checklist

✓ W key moves forward
✓ S key moves backward
✓ A key moves left
✓ D key moves right
✓ Q key moves down
✓ E key moves up
✓ Mouse movement rotates camera
✓ ESC closes program
✓ No error messages in console
✓ Smooth frame rate (60+ FPS)

---

## PHASE 7: Documentation and Submission (30 minutes)

### Step 1: Take Screenshot

While program is running:
- Position camera for good view of scene
- Use system screenshot tool (PrintScreen, Cmd+Shift+3, etc.)
- Save as `scene_screenshot.png`

### Step 2: Organize Files

```
project4/
├── main.cpp
├── vertex_shader.glsl
├── fragment_shader.glsl
├── README.md
├── INSTRUCTIONS.txt
├── CMakeLists.txt
└── scene_screenshot.png
```

### Step 3: Create BitBucket Repository

```bash
# Initialize git
git init

# Add files
git add .
git commit -m "Initial commit - OpenGL Stairwell Scene"

# Create repository on BitBucket
# Follow BitBucket instructions to push

# Push to remote
git remote add origin <your-bitbucket-url>
git push -u origin master
```

### Step 4: Grant Instructor Access

1. Go to repository settings on BitBucket
2. Navigate to "User and group access"
3. Add instructor's BitBucket username
4. Grant "Read" or "Write" access

---

## COMMON ISSUES AND SOLUTIONS

### Issue: "Cannot find -lGL"
**Solution:**
```bash
sudo apt-get install libgl1-mesa-dev
```

### Issue: "undefined reference to `glfwInit'"
**Solution:** Link libraries in correct order: `-lGLEW -lglfw -lGL`

### Issue: Black screen
**Solution:**
- Check shader compilation (look for errors in console)
- Verify camera position: `glm::vec3(0.0f, 1.5f, 5.0f)`
- Ensure depth testing is enabled: `glEnable(GL_DEPTH_TEST)`

### Issue: "error: 'value_ptr' is not a member of 'glm'"
**Solution:** Add include: `#include <glm/gtc/type_ptr.hpp>`

### Issue: Bricks look wrong
**Solution:** Check brick loop bounds and position calculations

---

## OPTIMIZATION TIPS

1. **Reduce brick count** if performance is slow:
   ```cpp
   for (int row = 0; row < 15; row++) {  // Was 20
       int bricksInRow = 6;  // Was 8
   ```

2. **Use instancing** for better performance (advanced):
   - Render all bricks in one draw call
   - Requires instance arrays

3. **Add back-face culling**:
   ```cpp
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);
   ```

---

## ENHANCEMENT IDEAS

After completing the basic scene:

1. **Add textures** to bricks and door
2. **Implement shadows** using shadow mapping
3. **Add more detail** to signs (actual text)
4. **Multiple lights** for more realistic lighting
5. **Post-processing** effects (bloom, ambient occlusion)

---

## FINAL CHECKLIST

Before submission:

- [ ] Code compiles without errors
- [ ] All shader files present
- [ ] README.md complete with all sections
- [ ] INSTRUCTIONS.txt included
- [ ] Screenshot included
- [ ] BitBucket repository created
- [ ] Instructor has access to repository
- [ ] All transformations documented
- [ ] Camera diagram included in README
- [ ] Tested on target platform

---

## TIME ESTIMATE

Total time: **~3.5 hours**

- Environment setup: 30 min
- Understanding: 15 min
- Shaders: 20 min
- Main code: 90 min
- Testing: 20 min
- Documentation: 30 min
- Submission: 15 min

---

**Congratulations!** You now have a complete OpenGL scene recreation.
