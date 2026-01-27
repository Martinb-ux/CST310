# Project 4: OpenGL Stairwell Scene Recreation

**Student Name:** [Your Name]  
**Course:** [Course Name]  
**Date:** January 27, 2026  

---

## Table of Contents
1. [Project Overview](#project-overview)
2. [Scene Analysis](#scene-analysis)
3. [Virtual Camera Setup](#virtual-camera-setup)
4. [Object Documentation](#object-documentation)
5. [Shader Implementation](#shader-implementation)
6. [Compilation and Execution](#compilation-and-execution)
7. [References](#references)

---

## Project Overview

This project recreates a stairwell scene from a photograph using OpenGL primitives, transformations, and lighting. The scene includes:
- Brick walls with realistic pattern
- Metal door with window and handle
- Emergency signage
- Overhead lighting
- Concrete floor and ceiling

The implementation uses modern OpenGL (3.3 Core Profile) with GLSL shaders for realistic lighting effects using the Phong illumination model.

---

## Scene Analysis

### Original Scene Characteristics

The photograph shows a stairwell corridor with the following key features:

1. **Left Wall**: Orange-red brick wall with traditional running bond pattern
2. **Right Wall**: Partial brick wall at an angle (perspective)
3. **Center**: Dark gray/charcoal metal door with:
   - Vertical rectangular window (wire glass)
   - Horizontal handle (brushed metal)
   - Metal frame with horizontal header
4. **Signage**: Two red safety signs
   - Emergency phone location sign (left)
   - East Stairs Floor 2 sign (center-left)
5. **Lighting**: Rectangular fluorescent fixture above door
6. **Floor**: Light gray concrete
7. **Ceiling**: Off-white/light gray

### Perspective and Viewing Angle

The photograph is taken from:
- **Position**: Slightly left of center, approximately 5-6 feet back from door
- **Height**: Adult eye level (~5-6 feet)
- **Angle**: Slightly looking up at the door and signs
- **Field of View**: Approximately 45-50 degrees

---

## Virtual Camera Setup

### Camera Position and Orientation

```cpp
glm::vec3 cameraPos = glm::vec3(0.0f, 1.5f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
```

**Camera Parameters:**
- **Position**: (0.0, 1.5, 5.0) in world coordinates
  - X: 0.0 (centered horizontally)
  - Y: 1.5 meters (approximately 5 feet - eye level)
  - Z: 5.0 meters (5 meters back from scene)
- **Front Vector**: (0, 0, -1) - looking toward negative Z axis
- **Up Vector**: (0, 1, 0) - Y-axis is up

### Projection Matrix

```cpp
glm::mat4 projection = glm::perspective(
    glm::radians(45.0f),                          // Field of view
    (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,   // Aspect ratio
    0.1f,                                         // Near clipping plane
    100.0f                                        // Far clipping plane
);
```

**Projection Parameters:**
- **Field of View**: 45 degrees (vertical)
- **Aspect Ratio**: 1200/900 = 1.33 (4:3)
- **Near Plane**: 0.1 units
- **Far Plane**: 100 units

### View Matrix

```cpp
glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
```

The view matrix transforms world coordinates to camera/view space using:
- Camera position
- Target point (camera position + front vector)
- Up direction

### Camera Diagram

```
                    Y (Up)
                    |
                    |
                    |
        Far Plane   |     Near Plane
            |       |        |
            |       |        |
    --------|-------|--------|---------> Z (Forward)
            |       |        |
            |       |        |
            |      Camera    |
            |    (0,1.5,5)   |
            |                |
           FOV=45°          Scene
                           (Z=0)


Top View:
                    
    Left Wall          Door          Right Wall
       |                |                |
       |                |                |
    -------          -------          -------
       |                |                |
       |                |                |
                        ^
                        |
                     Camera
                   (0, 1.5, 5)
```

### Camera Controls

**Keyboard Controls:**
- **W**: Move forward
- **S**: Move backward
- **A**: Move left
- **D**: Move right
- **Q**: Move down
- **E**: Move up
- **ESC**: Exit application

**Mouse Controls:**
- Mouse movement controls camera orientation
- Horizontal movement: Yaw rotation
- Vertical movement: Pitch rotation
- Pitch clamped between -89° and +89° to prevent gimbal lock

---

## Object Documentation

### 1. Brick Wall

#### Mathematical Characteristics

Each brick is a rectangular prism (cuboid) with dimensions:
- **Width (X)**: 0.4 units
- **Height (Y)**: 0.15 units
- **Depth (Z)**: 0.2 units
- **Mortar Gap**: 0.02 units between bricks

**Running Bond Pattern:**
- Each alternate row is offset by half a brick width (0.2 units)
- Creates traditional masonry appearance

**Position Formula:**
```
For row r and column c:
x = base_x + offset(r) + c * (brick_width + mortar_gap)
y = base_y + r * (brick_height + mortar_gap)
z = base_z

where offset(r) = (r % 2 == 0) ? 0.0 : brick_width / 2.0
```

#### Primitives Used

- **Primitive Type**: Cube (GL_TRIANGLES)
- **Number of vertices**: 36 per brick (6 faces × 2 triangles × 3 vertices)
- **Total bricks**: ~200 (20 rows × 8 columns on left wall + 20 rows × 4 columns on right wall)

#### Transformations

For each brick at position (r, c):

1. **Translation**:
   ```cpp
   glm::translate(model, glm::vec3(x, y, z));
   ```
   Positions the brick in the wall grid

2. **Scaling**:
   ```cpp
   glm::scale(model, glm::vec3(brickWidth, brickHeight, brickDepth));
   ```
   Sizes the unit cube to brick dimensions

**Left Wall Configuration:**
- Base position: (-4.0, -1.0, -2.0)
- 20 rows × 8 columns
- Running bond offset applied to alternate rows

**Right Wall Configuration:**
- Base position: (1.5, -1.0, -2.0)
- 20 rows × 4 columns
- Additional Z-offset per column for perspective: `z = -2.0 + col * 0.3`

#### Shader Properties

- **Color**: RGB(0.7, 0.35, 0.2) - Terracotta/orange-red
- **Lighting**: Full Phong illumination
  - Ambient: Provides base brick color
  - Diffuse: Creates depth between bricks
  - Specular: Minimal (bricks are matte)

### 2. Door

#### Mathematical Characteristics

The door is composed of multiple rectangular components:

**Main Door Body:**
- Width: 1.2 units
- Height: 2.8 units
- Depth: 0.1 units
- Position: (0.5, 0.5, -1.9)

**Door Window:**
- Width: 0.25 units
- Height: 0.8 units
- Depth: 0.05 units
- Position: (0.5, 1.2, -1.85) - offset forward from door

**Door Handle:**
- Width: 0.15 units
- Height: 0.05 units
- Depth: 0.1 units
- Position: (0.9, 0.5, -1.8) - right side of door

**Door Frame (Header):**
- Width: 1.3 units
- Height: 0.1 units
- Depth: 0.15 units
- Position: (0.5, 1.95, -1.85) - above door

#### Primitives Used

- **Primitive Type**: Cube (GL_TRIANGLES) for each component
- **Components**: 4 separate cubes
  1. Door body
  2. Window
  3. Handle
  4. Frame header

#### Transformations

**Door Body:**
```cpp
model = glm::translate(model, glm::vec3(0.5f, 0.5f, -1.9f));
model = glm::scale(model, glm::vec3(1.2f, 2.8f, 0.1f));
```
- Translation positions door centered slightly right
- Scale creates tall, flat rectangle

**Window:**
```cpp
model = glm::translate(model, glm::vec3(0.5f, 1.2f, -1.85f));
model = glm::scale(model, glm::vec3(0.25f, 0.8f, 0.05f));
```
- Positioned in upper portion of door
- Offset slightly forward (Z: -1.85 vs -1.9) to appear on door surface

**Handle:**
```cpp
model = glm::translate(model, glm::vec3(0.9f, 0.5f, -1.8f));
model = glm::scale(model, glm::vec3(0.15f, 0.05f, 0.1f));
```
- Positioned on right side at waist height
- Small, elongated shape

**Frame:**
```cpp
model = glm::translate(model, glm::vec3(0.5f, 1.95f, -1.85f));
model = glm::scale(model, glm::vec3(1.3f, 0.1f, 0.15f));
```
- Horizontal bar above door
- Slightly wider than door body

#### Shader Properties

**Door Body:**
- Color: RGB(0.2, 0.2, 0.2) - Dark charcoal gray
- Material: Matte metal
- Specular: Medium (0.4) for subtle metallic sheen

**Window:**
- Color: RGB(0.6, 0.7, 0.75) - Light blue-gray (glass effect)
- Material: Semi-reflective
- Specular: High for glass appearance

**Handle:**
- Color: RGB(0.75, 0.75, 0.75) - Brushed metal
- Material: Metallic
- Specular: High (0.5+) for shiny metal

**Frame:**
- Color: RGB(0.6, 0.6, 0.6) - Medium gray metal
- Material: Painted metal
- Specular: Medium

### 3. Emergency Signs

#### Mathematical Characteristics

Two identical square signs with different positions:

**Sign Dimensions:**
- Width: 0.35 units
- Height: 0.35 units
- Depth: 0.02 units (thin, flat)

**Emergency Phone Sign:**
- Position: (-1.2, 1.0, -1.95)

**East Stairs Sign:**
- Position: (-0.4, 1.0, -1.95)

**Text Representation:**
- White rectangles on red background
- Positioned slightly forward (Z: -1.93) to appear on sign surface

#### Primitives Used

- **Primitive Type**: Cube (GL_TRIANGLES)
- **Total Components**: 4
  - 2 red sign backgrounds
  - 2 white text areas

#### Transformations

**Sign Backgrounds:**
```cpp
model = glm::translate(model, glm::vec3(x, 1.0f, -1.95f));
model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.02f));
```
- Translation positions signs on wall at consistent height
- Scale creates thin, square plates

**Text Areas (Simplified):**
```cpp
// Emergency phone text
model = glm::translate(model, glm::vec3(-1.2f, 0.95f, -1.93f));
model = glm::scale(model, glm::vec3(0.28f, 0.15f, 0.01f));

// East stairs "2"
model = glm::translate(model, glm::vec3(-0.4f, 0.95f, -1.93f));
model = glm::scale(model, glm::vec3(0.15f, 0.2f, 0.01f));
```
- Positioned slightly lower and forward
- Smaller dimensions to fit on sign
- Very thin (0.01) to appear as surface detail

#### Shader Properties

**Sign Background:**
- Color: RGB(0.7, 0.0, 0.0) - Safety red
- Material: Matte painted metal
- Specular: Low (signs are not reflective)

**Text Areas:**
- Color: RGB(0.9, 0.9, 0.85) - Off-white/cream
- Material: Painted or vinyl lettering
- Specular: Very low (matte finish)

### 4. Floor

#### Mathematical Characteristics

Large rectangular plane:
- Width: 10.0 units
- Height: 0.1 units (thin slab)
- Depth: 8.0 units
- Position: (0.0, -1.1, -1.0)

#### Primitives Used

- **Primitive Type**: Cube (GL_TRIANGLES)
- **Purpose**: Single large flat cube representing concrete floor

#### Transformations

```cpp
model = glm::translate(model, glm::vec3(0.0f, -1.1f, -1.0f));
model = glm::scale(model, glm::vec3(10.0f, 0.1f, 8.0f));
```

**Explanation:**
- **Translation**: Positions floor below camera eye level
  - Y: -1.1 places it about 8.5 feet below camera (realistic floor position)
  - X, Z: Centered to extend equally in all directions
- **Scale**: Creates large, flat surface
  - X: 10 units wide (extends beyond visible area)
  - Y: 0.1 thin slab (realistic floor thickness)
  - Z: 8 units deep (from near to far)

#### Shader Properties

- Color: RGB(0.5, 0.5, 0.52) - Light gray concrete
- Material: Concrete (matte)
- Specular: Very low (0.1) - concrete is not reflective
- Diffuse: High - receives shadows well

### 5. Ceiling

#### Mathematical Characteristics

Large rectangular plane:
- Width: 10.0 units
- Height: 0.1 units (thin slab)
- Depth: 8.0 units
- Position: (0.0, 3.9, -1.0)

#### Primitives Used

- **Primitive Type**: Cube (GL_TRIANGLES)
- **Purpose**: Single large flat cube representing ceiling

#### Transformations

```cpp
model = glm::translate(model, glm::vec3(0.0f, 3.9f, -1.0f));
model = glm::scale(model, glm::vec3(10.0f, 0.1f, 8.0f));
```

**Explanation:**
- **Translation**: Positions ceiling above scene
  - Y: 3.9 places it approximately 8 feet above floor (realistic ceiling height)
  - X, Z: Centered to match floor dimensions
- **Scale**: Identical to floor for consistent room dimensions

#### Shader Properties

- Color: RGB(0.85, 0.85, 0.85) - Off-white/light gray
- Material: Painted drywall or suspended ceiling
- Specular: Very low (matte paint)
- Ambient response: High (bright surfaces)

### 6. Light Fixture

#### Mathematical Characteristics

Two-component assembly:

**Housing:**
- Width: 1.0 units
- Height: 0.15 units
- Depth: 0.3 units
- Position: (0.5, 3.6, -1.9)

**Light Panel:**
- Width: 0.9 units
- Height: 0.08 units
- Depth: 0.25 units
- Position: (0.5, 3.55, -1.88)

#### Primitives Used

- **Primitive Type**: Cube (GL_TRIANGLES)
- **Components**: 2 cubes
  1. White housing/frame
  2. Bright emitting panel

#### Transformations

**Housing:**
```cpp
model = glm::translate(model, glm::vec3(0.5f, 3.6f, -1.9f));
model = glm::scale(model, glm::vec3(1.0f, 0.15f, 0.3f));
```
- Positioned on ceiling above door
- Elongated horizontally

**Light Panel:**
```cpp
model = glm::translate(model, glm::vec3(0.5f, 3.55f, -1.88f));
model = glm::scale(model, glm::vec3(0.9f, 0.08f, 0.25f));
```
- Positioned slightly below and forward of housing
- Slightly smaller to fit within frame

#### Shader Properties

**Housing:**
- Color: RGB(0.95, 0.95, 0.9) - White/cream
- Material: Plastic or painted metal
- Specular: Medium (0.3) - slight sheen

**Light Panel:**
- Color: RGB(1.0, 0.98, 0.85) - Warm white/yellow
- Material: Diffuser (translucent plastic)
- Specular: Low (diffuse surface)
- Note: This is the visual representation; actual lighting comes from uniform lightPos

---

## Shader Implementation

### Vertex Shader (vertex_shader.glsl)

#### Purpose
Transforms vertex positions from object space to clip space and prepares data for lighting calculations.

#### Key Operations

1. **Position Transformation**:
   ```glsl
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   ```
   Applies full transformation pipeline:
   - Model matrix: Object space → World space
   - View matrix: World space → Camera space
   - Projection matrix: Camera space → Clip space

2. **Fragment Position Calculation**:
   ```glsl
   FragPos = vec3(model * vec4(aPos, 1.0));
   ```
   Converts vertex position to world space for lighting calculations

3. **Normal Transformation**:
   ```glsl
   Normal = mat3(transpose(inverse(model))) * aNormal;
   ```
   Uses normal matrix to correctly transform normals:
   - Handles non-uniform scaling
   - Preserves perpendicularity
   - Normalizes in fragment shader

#### Input Attributes
- `layout (location = 0) in vec3 aPos`: Vertex position
- `layout (location = 1) in vec3 aNormal`: Vertex normal

#### Output Variables
- `out vec3 FragPos`: Fragment position in world space
- `out vec3 Normal`: Normal vector in world space

#### Uniforms
- `uniform mat4 model`: Model transformation matrix
- `uniform mat4 view`: View transformation matrix
- `uniform mat4 projection`: Projection transformation matrix

### Fragment Shader (fragment_shader.glsl)

#### Purpose
Implements Phong lighting model to calculate realistic lighting for each fragment.

#### Phong Lighting Model Components

The Phong model combines three lighting components:

1. **Ambient Lighting**:
   ```glsl
   float ambientStrength = 0.3;
   vec3 ambient = ambientStrength * lightColor;
   ```
   - Provides base illumination
   - Simulates indirect/reflected light
   - Prevents completely black shadows
   - Strength: 0.3 (30% of light color)

2. **Diffuse Lighting**:
   ```glsl
   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(lightPos - FragPos);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = diff * lightColor;
   ```
   - Simulates directional light scattering
   - Based on Lambert's cosine law
   - Intensity proportional to angle between normal and light direction
   - Uses dot product: I = max(N · L, 0)

3. **Specular Lighting**:
   ```glsl
   float specularStrength = 0.4;
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   vec3 specular = specularStrength * spec * lightColor;
   ```
   - Creates shiny highlights
   - Based on reflection vector
   - Uses Phong reflection model
   - Shininess: 32 (controls highlight size)
   - Strength: 0.4 (40% contribution)

#### Final Color Calculation

```glsl
vec3 result = (ambient + diffuse + specular) * objectColor;
FragColor = vec4(result, 1.0);
```

Combines all three components and modulates by object's base color.

#### Input Variables
- `in vec3 FragPos`: Fragment position from vertex shader
- `in vec3 Normal`: Normal vector from vertex shader

#### Output Variables
- `out vec4 FragColor`: Final RGBA color

#### Uniforms
- `uniform vec3 lightPos`: Light source position (0.0, 3.5, 0.0)
- `uniform vec3 viewPos`: Camera position
- `uniform vec3 lightColor`: Light color RGB(1.0, 0.95, 0.8) - warm white
- `uniform vec3 objectColor`: Object's base color

#### Why Phong Lighting?

**Advantages:**
1. **Realism**: Creates believable 3D appearance with depth
2. **Efficiency**: Computed per-fragment, good performance
3. **Flexibility**: Easy to adjust ambient, diffuse, and specular contributions
4. **Material Variety**: Different objects can have different specular properties

**Alternative Considered:**
- Flat shading: Too simple, no depth perception
- Gouraud shading: Per-vertex lighting, lower quality highlights
- PBR (Physically Based Rendering): More complex, unnecessary for this scene

---

## Compilation and Execution

### Software Requirements

**Required Libraries:**
1. **GLFW 3.3+**: Window and input management
   - Creates OpenGL context
   - Handles keyboard/mouse input
   - Manages window lifecycle

2. **GLEW 2.1+**: OpenGL extension loading
   - Loads modern OpenGL functions
   - Ensures cross-platform compatibility

3. **GLM 0.9.9+**: OpenGL Mathematics
   - Matrix operations (transformations)
   - Vector operations
   - Camera calculations

4. **OpenGL 3.3+**: Graphics API
   - Core profile required
   - Hardware with OpenGL 3.3 support

**Compiler:**
- GCC 7+ or Clang 6+ (Linux/Mac)
- MSVC 2017+ (Windows)
- C++11 standard or higher

### Installation Instructions

#### Ubuntu/Debian Linux:
```bash
sudo apt-get update
sudo apt-get install build-essential
sudo apt-get install libglfw3-dev
sudo apt-get install libglew-dev
sudo apt-get install libglm-dev
```

#### macOS (using Homebrew):
```bash
brew install glfw
brew install glew
brew install glm
```

#### Windows:
1. Download GLFW from https://www.glfw.org/download.html
2. Download GLEW from http://glew.sourceforge.net/
3. Download GLM from https://github.com/g-truc/glm/releases
4. Set up include and library paths in Visual Studio

### Compilation

#### Linux/macOS:
```bash
# Navigate to project directory
cd project4

# Compile with g++
g++ -std=c++11 main.cpp -o stairwell_scene \
    -lGL -lGLEW -lglfw -lm

# Alternative: Using pkg-config
g++ -std=c++11 main.cpp -o stairwell_scene \
    $(pkg-config --cflags --libs glfw3 glew) -lGL -lm
```

#### Using CMake (Recommended):
Create `CMakeLists.txt`:
```cmake
cmake_minimum_required(VERSION 3.10)
project(StairwellScene)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLEW REQUIRED)

add_executable(stairwell_scene main.cpp)

target_link_libraries(stairwell_scene 
    OpenGL::GL 
    glfw 
    GLEW::GLEW
)
```

Then compile:
```bash
mkdir build
cd build
cmake ..
make
```

#### Windows (Visual Studio):
1. Create new C++ project
2. Add main.cpp, vertex_shader.glsl, fragment_shader.glsl
3. Configure include directories for GLFW, GLEW, GLM
4. Configure library directories
5. Link: opengl32.lib, glfw3.lib, glew32.lib
6. Build solution (Ctrl+Shift+B)

### Running the Program

#### Linux/macOS:
```bash
./stairwell_scene
```

#### Windows:
```bash
stairwell_scene.exe
```

Or run from Visual Studio (F5 or Ctrl+F5)

### Troubleshooting

**"Cannot find shader files" error:**
- Ensure `vertex_shader.glsl` and `fragment_shader.glsl` are in the same directory as executable
- Check file permissions (read access required)

**Black screen:**
- Verify OpenGL version: `glxinfo | grep "OpenGL version"` (Linux)
- Update graphics drivers
- Check shader compilation output in console

**Linker errors:**
- Verify all libraries are installed
- Check library paths in compiler command
- Ensure 64-bit libraries match 64-bit compiler (or both 32-bit)

**Window not responding:**
- Check graphics drivers
- Try windowed mode instead of fullscreen
- Reduce resolution

---

## Scene Coordinate System

### World Space Coordinate System

```
       Y (Up)
       |
       |
       |
       +--------> X (Right)
      /
     /
    Z (Toward Camera)

Origin (0,0,0) is at center of scene
```

### Object Positions Summary

| Object | X Position | Y Position | Z Position | Notes |
|--------|-----------|-----------|-----------|-------|
| Camera | 0.0 | 1.5 | 5.0 | Eye level, back from scene |
| Left Wall | -4.0 to -1.0 | -1.0 to 2.9 | -2.0 | Brick wall |
| Right Wall | 1.5 to 3.0 | -1.0 to 2.9 | -2.0 to 0.2 | Angled |
| Door | 0.5 | -0.9 to 1.9 | -1.9 | Center-right |
| Signs | -1.2, -0.4 | 0.825 to 1.175 | -1.95 | On left wall |
| Floor | -5.0 to 5.0 | -1.15 to -1.05 | -5.0 to 3.0 | Below all |
| Ceiling | -5.0 to 5.0 | 3.85 to 3.95 | -5.0 to 3.0 | Above all |
| Light | 0.5 | 3.55 to 3.675 | -1.9 | On ceiling |

---

## Transformation Hierarchy

### Transformation Order

For each object, transformations are applied in this order:

1. **Scaling**: Size the object
2. **Rotation**: Orient the object (not used in this scene)
3. **Translation**: Position the object

This is represented by the transformation matrix:
```
M = T × R × S
```

Where:
- T = Translation matrix
- R = Rotation matrix  
- S = Scaling matrix

### Example: Door Rendering

```cpp
glm::mat4 model = glm::mat4(1.0f);  // Identity matrix

// Step 1: Scale (creates door dimensions)
// Scales unit cube to 1.2 × 2.8 × 0.1
model = glm::scale(model, glm::vec3(1.2f, 2.8f, 0.1f));

// Step 2: Translate (positions door in scene)
// Moves door to (0.5, 0.5, -1.9)
model = glm::translate(model, glm::vec3(0.5f, 0.5f, -1.9f));

// Note: In GLM, operations are applied right-to-left
// So we actually write translate THEN scale, but they execute scale THEN translate
```

### Coordinate Space Transformations

```
Object Space (Local)
      ↓ [Model Matrix]
World Space
      ↓ [View Matrix]
Camera/View Space
      ↓ [Projection Matrix]
Clip Space
      ↓ [Perspective Division]
Normalized Device Coordinates (-1 to 1)
      ↓ [Viewport Transform]
Screen Space (pixels)
```

---

## Advanced Features

### Camera Movement System

The program implements a first-person camera with:

1. **WASD Movement**: Translates camera position
2. **QE Vertical**: Moves camera up/down
3. **Mouse Look**: Rotates camera view
4. **Delta Time**: Ensures consistent movement speed regardless of frame rate

### Lighting System

**Light Source:**
- Type: Point light
- Position: (0.0, 3.5, 0.0) - ceiling center
- Color: RGB(1.0, 0.95, 0.8) - warm white
- Attenuation: None (simplified for indoor scene)

**Material Properties:**
Each object has different material response to lighting based on its color and surface properties.

### Future Enhancements

Potential improvements for future iterations:

1. **Texture Mapping**: Add realistic brick, metal, and concrete textures
2. **Normal Mapping**: Enhanced surface detail without additional geometry
3. **Multiple Lights**: Additional light sources for more realistic illumination
4. **Shadows**: Shadow mapping for depth and realism
5. **Text Rendering**: Actual text on signs instead of colored rectangles
6. **Post-Processing**: Bloom, ambient occlusion, or other effects

---

## Performance Considerations

### Optimization Techniques Used

1. **Static Geometry**: All vertices uploaded once at initialization
2. **Instancing**: Could be used for bricks (not implemented)
3. **Culling**: Back-face culling enabled
4. **Depth Testing**: Early fragment rejection

### Performance Metrics

Expected performance on modern hardware:
- **FPS**: 60+ fps (uncapped)
- **Draw Calls**: ~210 per frame (200 bricks + 10 other objects)
- **Vertices**: ~7,560 per frame (210 objects × 36 vertices)
- **Memory**: <10 MB VRAM

### Bottlenecks

Current bottlenecks:
1. **Individual brick rendering**: Each brick is a separate draw call
2. **No instancing**: Could reduce draw calls significantly
3. **Overdraw**: Some fragments drawn then occluded

---

## Code Organization

### File Structure

```
project4/
├── main.cpp                 # Main application code
├── vertex_shader.glsl       # Vertex shader source
├── fragment_shader.glsl     # Fragment shader source
├── README.md               # This documentation
├── INSTRUCTIONS.txt        # Build and run instructions
├── CMakeLists.txt          # CMake build configuration (optional)
└── screenshots/            # Execution screenshots
    └── scene_render.png
```

### Function Breakdown

**Initialization Functions:**
- `main()`: Entry point, initialization, main loop
- `loadShader()`: Compiles and links shader programs

**Rendering Functions:**
- `renderBrickWall()`: Renders all brick wall geometry
- `renderDoor()`: Renders door assembly
- `renderSigns()`: Renders emergency signs
- `renderFloor()`: Renders floor plane
- `renderCeiling()`: Renders ceiling plane
- `renderLight()`: Renders light fixture

**Input Functions:**
- `processInput()`: Keyboard input handling
- `mouse_callback()`: Mouse movement handling
- `framebuffer_size_callback()`: Window resize handling

---

## Learning Outcomes

This project demonstrates:

1. **3D Graphics Pipeline**: Understanding of vertex to fragment transformation
2. **Transformation Matrices**: Application of translation, rotation, and scaling
3. **Lighting Models**: Implementation of Phong illumination
4. **Shader Programming**: GLSL vertex and fragment shaders
5. **Camera Systems**: Virtual camera implementation
6. **Scene Composition**: Breaking complex scenes into primitive components
7. **OpenGL API**: Modern OpenGL 3.3+ core profile usage

---

## References

1. **OpenGL Programming Guide** (Red Book), 9th Edition
   - Transformation matrices
   - Lighting models
   
2. **LearnOpenGL.com** by Joey de Vries
   - Modern OpenGL tutorials
   - Camera implementation
   - Phong lighting
   - https://learnopengl.com/

3. **OpenGL SuperBible**, 7th Edition
   - Shader programming
   - Advanced rendering techniques

4. **GLFW Documentation**
   - Window management
   - Input handling
   - https://www.glfw.org/documentation.html

5. **GLM Documentation**
   - Matrix mathematics
   - Vector operations
   - https://glm.g-truc.net/

6. **OpenGL 3.3 Core Specification**
   - API reference
   - https://www.opengl.org/registry/

7. **Real-Time Rendering**, 4th Edition by Tomas Akenine-Möller et al.
   - Lighting theory
   - Transformation pipelines

---

## Appendix A: Keyboard Reference Card

| Key | Action |
|-----|--------|
| W | Move camera forward |
| S | Move camera backward |
| A | Move camera left |
| D | Move camera right |
| Q | Move camera down |
| E | Move camera up |
| Mouse | Look around |
| ESC | Exit program |

---

## Appendix B: Color Palette

| Object | RGB Values | Hex | Description |
|--------|-----------|-----|-------------|
| Bricks | (0.7, 0.35, 0.2) | #B35933 | Terracotta |
| Door | (0.2, 0.2, 0.2) | #333333 | Charcoal |
| Window | (0.6, 0.7, 0.75) | #99B2BF | Blue-gray |
| Handle | (0.75, 0.75, 0.75) | #BFBFBF | Brushed metal |
| Signs | (0.7, 0.0, 0.0) | #B30000 | Safety red |
| Sign Text | (0.9, 0.9, 0.85) | #E6E6D9 | Off-white |
| Floor | (0.5, 0.5, 0.52) | #808085 | Gray concrete |
| Ceiling | (0.85, 0.85, 0.85) | #D9D9D9 | Light gray |
| Light | (1.0, 0.98, 0.85) | #FFF9D9 | Warm white |

---

## Appendix C: Mathematical Formulas

### Perspective Projection

```
              f
P = ─────────────────
    aspect × tan(fov/2)

where:
f = focal length
aspect = width / height
fov = field of view (in radians)
```

### Phong Lighting Components

**Ambient:**
```
I_ambient = k_a × I_light
```

**Diffuse:**
```
I_diffuse = k_d × I_light × max(N · L, 0)
```

**Specular:**
```
I_specular = k_s × I_light × max(R · V, 0)^α
```

**Total:**
```
I_total = I_ambient + I_diffuse + I_specular
```

### Normal Matrix

```
N_matrix = (M_model^-1)^T
```
Ensures normals remain perpendicular under non-uniform scaling.

---

## Submission Checklist

- [x] Complete documented source code (main.cpp)
- [x] Vertex shader (vertex_shader.glsl)
- [x] Fragment shader (fragment_shader.glsl)
- [x] Comprehensive README.md documentation
- [x] INSTRUCTIONS.txt with compilation/execution steps
- [x] Mathematical characteristics for each object
- [x] Primitive descriptions
- [x] Transformation explanations
- [x] Shader explanations with reasoning
- [x] Virtual camera documentation with diagram
- [x] Screenshot of running program
- [x] Code comments throughout
- [x] BitBucket repository access for instructor

---

**End of Documentation**
