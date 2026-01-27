# OpenGL Stairwell Scene - Quick Reference

## Object Positions and Dimensions

| Object | Position (X, Y, Z) | Scale (W, H, D) | Color (R, G, B) |
|--------|-------------------|-----------------|-----------------|
| Camera | (0.0, 1.5, 5.0) | - | - |
| Door Body | (0.5, 0.5, -1.9) | (1.2, 2.8, 0.1) | (0.2, 0.2, 0.2) |
| Door Window | (0.5, 1.2, -1.85) | (0.25, 0.8, 0.05) | (0.6, 0.7, 0.75) |
| Door Handle | (0.9, 0.5, -1.8) | (0.15, 0.05, 0.1) | (0.75, 0.75, 0.75) |
| Door Frame | (0.5, 1.95, -1.85) | (1.3, 0.1, 0.15) | (0.6, 0.6, 0.6) |
| Sign 1 (Phone) | (-1.2, 1.0, -1.95) | (0.35, 0.35, 0.02) | (0.7, 0.0, 0.0) |
| Sign 2 (Stairs) | (-0.4, 1.0, -1.95) | (0.35, 0.35, 0.02) | (0.7, 0.0, 0.0) |
| Floor | (0.0, -1.1, -1.0) | (10.0, 0.1, 8.0) | (0.5, 0.5, 0.52) |
| Ceiling | (0.0, 3.9, -1.0) | (10.0, 0.1, 8.0) | (0.85, 0.85, 0.85) |
| Light Housing | (0.5, 3.6, -1.9) | (1.0, 0.15, 0.3) | (0.95, 0.95, 0.9) |
| Light Panel | (0.5, 3.55, -1.88) | (0.9, 0.08, 0.25) | (1.0, 0.98, 0.85) |

## Brick Wall Parameters

```cpp
brickWidth = 0.4f
brickHeight = 0.15f
brickDepth = 0.2f
mortarGap = 0.02f
brickColor = (0.7, 0.35, 0.2)

Left Wall: 20 rows × 8 columns
  Base: (-4.0, -1.0, -2.0)
  Offset: (row % 2) ? brickWidth/2 : 0

Right Wall: 20 rows × 4 columns
  Base: (1.5, -1.0, -2.0)
  Z-offset: +0.3 per column
```

## Lighting Parameters

```cpp
lightPos = (0.0, 3.5, 0.0)
lightColor = (1.0, 0.95, 0.8)
ambientStrength = 0.3
specularStrength = 0.4
shininess = 32
```

## Camera Controls

| Key | Action |
|-----|--------|
| W | Forward |
| S | Backward |
| A | Left |
| D | Right |
| Q | Down |
| E | Up |
| Mouse | Look |
| ESC | Exit |

## OpenGL Functions Quick Reference

### Initialization
```cpp
glfwInit();
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
glfwMakeContextCurrent(window);
glewInit();
glEnable(GL_DEPTH_TEST);
```

### Vertex Array Setup
```cpp
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);
glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
glVertexAttribPointer(location, size, type, normalized, stride, offset);
glEnableVertexAttribArray(location);
```

### Drawing
```cpp
glClearColor(r, g, b, a);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glUseProgram(shaderProgram);
glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
glUniform3fv(location, 1, glm::value_ptr(vector));
glBindVertexArray(VAO);
glDrawArrays(GL_TRIANGLES, 0, vertexCount);
glfwSwapBuffers(window);
glfwPollEvents();
```

## GLM Transformations

```cpp
// Identity matrix
glm::mat4 model = glm::mat4(1.0f);

// Translation
model = glm::translate(model, glm::vec3(x, y, z));

// Rotation
model = glm::rotate(model, glm::radians(angle), glm::vec3(x, y, z));

// Scaling
model = glm::scale(model, glm::vec3(sx, sy, sz));

// View matrix
glm::mat4 view = glm::lookAt(position, target, up);

// Projection matrix
glm::mat4 projection = glm::perspective(fov, aspect, near, far);
```

## Shader Uniforms

```cpp
// Get uniform location
GLint location = glGetUniformLocation(shaderProgram, "uniformName");

// Set matrix
glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));

// Set vec3
glUniform3fv(location, 1, glm::value_ptr(vector));
glUniform3f(location, x, y, z);

// Set float
glUniform1f(location, value);

// Set int
glUniform1i(location, value);
```

## Common Transformations

### Center an Object
```cpp
model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
```

### Move Object to Position
```cpp
model = glm::translate(model, glm::vec3(x, y, z));
```

### Scale Object
```cpp
model = glm::scale(model, glm::vec3(width, height, depth));
```

### Rotate Object
```cpp
model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
```

### Combine Transforms (Order Matters!)
```cpp
model = glm::mat4(1.0f);
model = glm::translate(model, position);  // Third (applied last)
model = glm::rotate(model, angle, axis);  // Second
model = glm::scale(model, scale);         // First (applied first)
```

## Compilation Commands

### Linux/macOS
```bash
# GCC with pkg-config
g++ -std=c++11 main.cpp -o scene \
    $(pkg-config --cflags --libs glfw3 glew) -lGL -lm

# CMake
mkdir build && cd build
cmake .. && make
```

### Windows (MinGW)
```bash
g++ -std=c++11 main.cpp -o scene.exe \
    -lglfw3 -lglew32 -lopengl32 -lgdi32
```

## Debugging Tips

### Check OpenGL Errors
```cpp
GLenum err;
while ((err = glGetError()) != GL_NO_ERROR) {
    std::cout << "OpenGL error: " << err << std::endl;
}
```

### Print Matrix
```cpp
for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
        std::cout << matrix[i][j] << " ";
    }
    std::cout << std::endl;
}
```

### Check Shader Compilation
```cpp
GLint success;
GLchar infoLog[512];
glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cout << "Shader compilation failed:\n" << infoLog << std::endl;
}
```

## File Structure
```
project4/
├── main.cpp                   # Main program
├── vertex_shader.glsl         # Vertex shader
├── fragment_shader.glsl       # Fragment shader
├── README.md                  # Documentation
├── INSTRUCTIONS.txt           # Build instructions
├── CMakeLists.txt            # CMake config
└── scene_screenshot.png      # Screenshot
```

## Common Errors and Fixes

| Error | Fix |
|-------|-----|
| Cannot find -lGL | `sudo apt-get install libgl1-mesa-dev` |
| undefined reference to glfwInit | Link in order: `-lGLEW -lglfw -lGL` |
| Black screen | Check shader compilation, verify glEnable(GL_DEPTH_TEST) |
| Shader compile error | Check GLSL version: `#version 330 core` |
| No objects visible | Check camera position and orientation |
| Linker errors | Verify library paths and 32/64-bit match |

## Performance Targets

- **FPS**: 60+ (uncapped)
- **Draw Calls**: ~210 per frame
- **Memory**: <100 MB RAM
- **VRAM**: <10 MB

## Testing Checklist

- [ ] Program compiles without warnings
- [ ] Window opens correctly
- [ ] All objects visible
- [ ] Lighting works (not flat)
- [ ] Camera moves with WASD
- [ ] Mouse look works
- [ ] ESC exits cleanly
- [ ] No console errors
- [ ] Frame rate smooth
- [ ] Scene matches photo

## Coordinate System
```
       Y (Up)
       |
       |
       |
       +--------> X (Right)
      /
     /
    Z (Forward/Toward Camera)

Origin: (0,0,0)
Camera: (0, 1.5, 5)
Scene Center: (0, 0, 0)
```
