 # Project 4: Stairwell Scene - OpenGL Rendering

## Table of Contents
1. [Project Overview](#project-overview)
2. [Scene Description](#scene-description)
3. [Object Documentation](#object-documentation)
4. [Virtual Camera](#virtual-camera)
5. [Shaders](#shaders)
6. [Building and Running](#building-and-running)
7. [Controls](#controls)

---

## Project Overview

This project renders a 3D recreation of a real-world stairwell scene using OpenGL. The scene is based on a reference photograph (`realScene.png`) showing an interior stairwell corner with brick walls, a metal door, ceiling light, emergency signs, and safety equipment.

### Technologies Used
- **OpenGL 3.3 Core Profile**: Modern programmable pipeline
- **GLFW**: Window creation and input handling
- **GLEW**: OpenGL extension loading
- **GLM**: Mathematics library for transformations

---

## Scene Description

The scene depicts a corner of an interior stairwell with the following elements:
- Three brick walls forming a corner (back wall, left wall, right wall)
- Concrete floor
- Off-white ceiling
- Gray metal door with window, handle, and hydraulic closer
- Fluorescent ceiling light fixture
- Emergency phone sign (red with white text)
- East Stairs floor indicator sign (number "2")
- Fire alarm pull station

---

## Object Documentation

### 1. Back Wall

**Mathematical Characteristics:**
- Geometry: Planar surface (2D quad embedded in 3D space)
- Equation: z = 0 (plane perpendicular to Z-axis)
- Vertices: 4 points forming a rectangle
- Dimensions: 6.0m × 3.0m (width × height)

**Primitives Used:**
- Quad composed of 2 triangles (6 vertices total)
- Triangle 1: Bottom-left → Bottom-right → Top-right
- Triangle 2: Bottom-left → Top-right → Top-left

**Transformations:**
- Identity matrix (no transformation needed)
- Positioned at world origin by vertex coordinates

**Color:** RGB(0.65, 0.35, 0.28) - Reddish-brown brick

---

### 2. Left Wall

**Mathematical Characteristics:**
- Geometry: Planar surface perpendicular to X-axis
- Equation: x = -3.0 (plane at left boundary)
- Vertices: 4 points forming a rectangle
- Dimensions: 6.0m × 3.0m (depth × height)

**Primitives Used:**
- Quad composed of 2 triangles (6 vertices)

**Transformations:**
- Translation: Positioned at x = -roomWidth/2
- No rotation needed (defined by vertex positions)

**Color:** RGB(0.65, 0.35, 0.28) - Reddish-brown brick

---

### 3. Right Wall

**Mathematical Characteristics:**
- Geometry: Planar surface perpendicular to X-axis
- Equation: x = 3.0 (plane at right boundary)
- Vertices: 4 points forming a rectangle
- Dimensions: 6.0m × 3.0m (depth × height)

**Primitives Used:**
- Quad composed of 2 triangles (6 vertices)

**Transformations:**
- Translation: Positioned at x = roomWidth/2

**Color:** RGB(0.65, 0.35, 0.28) - Reddish-brown brick

---

### 4. Floor

**Mathematical Characteristics:**
- Geometry: Horizontal planar surface
- Equation: y = 0 (ground plane)
- Vertices: 4 points at corners
- Dimensions: 6.0m × 6.0m (width × depth)

**Primitives Used:**
- Quad composed of 2 triangles (6 vertices)

**Transformations:**
- Identity matrix (positioned at y = 0)

**Color:** RGB(0.75, 0.72, 0.65) - Beige concrete

---

### 5. Ceiling

**Mathematical Characteristics:**
- Geometry: Horizontal planar surface
- Equation: y = 3.0 (ceiling height)
- Dimensions: 6.0m × 6.0m

**Primitives Used:**
- Quad composed of 2 triangles (6 vertices)

**Transformations:**
- Translation: Positioned at y = roomHeight (3.0m)

**Color:** RGB(0.9, 0.88, 0.85) - Off-white

---

### 6. Door

**Mathematical Characteristics:**
- Geometry: Rectangular prism (3D box)
- Center: (0.8, 1.05, 0.035)
- Dimensions: 0.9m × 2.1m × 0.05m (width × height × thickness)

**Primitives Used:**
- Box: 6 faces × 2 triangles = 12 triangles (36 vertices)

**Transformations:**
- Translation: T(0.8, 1.05, 0.035) - Offset right of center, positioned on back wall
- Scaling: S(0.9, 2.1, 0.05) - Door dimensions
- No rotation

**Color:** RGB(0.45, 0.47, 0.50) - Gray metal

---

### 7. Door Frame

**Mathematical Characteristics:**
- Geometry: Three rectangular prisms (left, right, top)
- Frame width: 0.08m
- Frame depth: 0.1m

**Primitives Used:**
- 3 boxes × 36 vertices = 108 vertices total

**Transformations:**
- Left frame: T(doorX - doorWidth/2 - frameWidth/2, doorHeight/2, frameDepth/2)
- Right frame: T(doorX + doorWidth/2 + frameWidth/2, doorHeight/2, frameDepth/2)
- Top frame: T(doorX, doorHeight + frameWidth/2, frameDepth/2)

**Color:** RGB(0.35, 0.37, 0.40) - Dark gray

---

### 8. Door Window

**Mathematical Characteristics:**
- Geometry: Thin rectangular prism
- Dimensions: 0.15m × 0.5m × 0.01m
- Position: Upper 70% of door height

**Primitives Used:**
- Box (36 vertices)

**Transformations:**
- Translation: T(doorX, doorHeight × 0.7, doorThick + 0.02)

**Color:** RGB(0.7, 0.85, 0.9) - Light blue tint (glass)

---

### 9. Door Handle

**Mathematical Characteristics:**
- Geometry: Small rectangular prism
- Dimensions: 0.12m × 0.04m × 0.06m

**Primitives Used:**
- Box (36 vertices)

**Transformations:**
- Translation: T(doorX + doorWidth/2 - 0.1, 1.0, doorThick + 0.04)
- Positioned on right side of door at handle height (~1m)

**Color:** RGB(0.75, 0.75, 0.78) - Silver

---

### 10. Door Closer

**Mathematical Characteristics:**
- Geometry: Rectangular prism (hydraulic arm)
- Dimensions: 0.25m × 0.06m × 0.08m

**Primitives Used:**
- Box (36 vertices)

**Transformations:**
- Translation: T(doorX + 0.1, doorHeight - 0.1, doorThick + 0.05)

**Color:** RGB(0.75, 0.75, 0.78) - Silver

---

### 11. Ceiling Light Fixture

**Mathematical Characteristics:**
- Geometry: Rectangular prism (fluorescent fixture)
- Dimensions: 1.2m × 0.08m × 0.15m
- Position: Suspended from ceiling

**Primitives Used:**
- Box (36 vertices)

**Transformations:**
- Translation: T(-0.5, roomHeight - 0.04, 1.5)
- Positioned slightly left of center

**Color:** RGB(0.95, 0.95, 0.92) - White (light emitting)

---

### 12. Emergency Phone Sign

**Mathematical Characteristics:**
- Geometry: Thin rectangular prism on wall surface
- Dimensions: 0.02m × 0.25m × 0.35m

**Primitives Used:**
- Box (36 vertices) for red background
- Box (36 vertices) for white text area

**Transformations:**
- Translation: T(-roomWidth/2 + 0.02, 1.5, 1.2)
- Positioned on left wall, facing into room

**Color:** RGB(0.85, 0.15, 0.15) - Red with white text

---

### 13. East Stairs Sign (Floor "2")

**Mathematical Characteristics:**
- Geometry: Thin rectangular prism
- Dimensions: 0.02m × 0.3m × 0.25m

**Primitives Used:**
- Box (36 vertices) for sign body
- Box (36 vertices) for number "2" area

**Transformations:**
- Translation: T(-roomWidth/2 + 0.02, 1.5, 0.5)

**Color:** RGB(0.85, 0.15, 0.15) - Red with white number

---

### 14. Fire Alarm Pull Station

**Mathematical Characteristics:**
- Geometry: Small box protruding from wall
- Dimensions: 0.04m × 0.12m × 0.1m

**Primitives Used:**
- Box (36 vertices) for body
- Box (36 vertices) for white stripe

**Transformations:**
- Translation: T(-roomWidth/2 + 0.02, 1.1, 1.5)

**Color:** RGB(0.9, 0.2, 0.2) - Red with white stripe

---

## Virtual Camera

### Camera Configuration

**Position:**
- Initial: (0.0, 1.7, 4.0) in world coordinates
- X = 0.0: Centered horizontally
- Y = 1.7: Eye level height (approximately 1.7 meters)
- Z = 4.0: Positioned 4 meters back from back wall

**Orientation:**
- Yaw: -90° (looking toward negative Z axis)
- Pitch: -5° (slightly looking down)
- Up vector: (0, 1, 0)

**Projection Parameters:**
- Type: Perspective projection
- Field of View: 60°
- Aspect Ratio: 16:9 (1280×720)
- Near Plane: 0.1 units
- Far Plane: 100.0 units

### Camera Diagram

```
                    TOP VIEW (Y-axis coming out of page)

                         Back Wall (z=0)
        ┌─────────────────────────────────────────┐
        │                  [Door]                 │
        │                                         │
   Left │                    ·                    │ Right
   Wall │                    │                    │ Wall
  (x=-3)│                    │                    │(x=3)
        │                    │                    │
        │                    │                    │
        │                    ▼                    │
        │               Camera Front              │
        │                                         │
        │                    △                    │
        │                 Camera                  │
        │              (0, 1.7, 4)                │
        │                                         │
        └─────────────────────────────────────────┘
                         z = 6 (Front)


                    SIDE VIEW (X-axis coming out of page)

        y=3  ┌─────────────────────────────────────┐ Ceiling
             │          [Light]                    │
             │                                     │
             │                                     │
             │   Door                              │
             │   ┌───┐                    △ Camera │
        y=1.7│   │   │                   /  Eye    │
             │   │   │                  /   Level  │
             │   │   │                 /           │
             │   └───┘                             │
        y=0  └─────────────────────────────────────┘ Floor
             z=0                                z=6
                    (Back)              (Front)


            VIEWING FRUSTUM (Perspective Projection)

                Near Plane (z = 0.1)
                     ┌───────┐
                    /         \
                   /           \
                  /    FOV      \
                 /    60°        \
                /       │         \
               /        │          \
              /         ▼           \
             /      Camera           \
            /     (Eye Point)         \
           /                           \
          └─────────────────────────────┘
                Far Plane (z = 100)
```

### View Matrix Calculation

The view matrix transforms world coordinates to camera space:

```
View Matrix = lookAt(cameraPos, cameraPos + cameraFront, cameraUp)
```

Where:
- `cameraPos` = (0, 1.7, 4)
- `cameraFront` = calculated from yaw and pitch angles
- `cameraUp` = (0, 1, 0)

### Projection Matrix Calculation

Perspective projection matrix:

```
Projection = perspective(fov, aspect, near, far)
           = perspective(60°, 1.778, 0.1, 100.0)
```

---

## Shaders

### Vertex Shader (`vertex.glsl`)

**Purpose:**
- Transform vertices from model space to clip space
- Pass attributes (color, normal, position) to fragment shader
- Calculate world-space position for lighting

**Key Operations:**

1. **Model Transformation:**
   ```glsl
   FragPos = vec3(model * vec4(aPos, 1.0));
   ```
   Converts local vertex position to world space.

2. **Normal Transformation:**
   ```glsl
   Normal = mat3(transpose(inverse(model))) * aNormal;
   ```
   Properly transforms normals (handles non-uniform scaling).

3. **MVP Transformation:**
   ```glsl
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   ```
   Full transformation pipeline: Model → View → Projection.

---

### Fragment Shader (`fragment.glsl`)

**Purpose:**
- Calculate final pixel color using Phong lighting model
- Combine ambient, diffuse, and specular components

**Lighting Model:**

1. **Ambient Component:**
   ```glsl
   vec3 ambient = 0.3 * lightColor;
   ```
   Base illumination simulating indirect light.

2. **Diffuse Component (Lambert's Law):**
   ```glsl
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = diff * lightColor;
   ```
   Light intensity based on surface angle to light.

3. **Specular Component (Phong):**
   ```glsl
   vec3 reflectDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);
   vec3 specular = 0.2 * spec * lightColor;
   ```
   Shiny highlights from reflected light.

4. **Final Color:**
   ```glsl
   vec3 result = (ambient + diffuse + specular) * Color;
   FragColor = vec4(result, 1.0);
   ```

---

## Building and Running

### Prerequisites

Install the required libraries:

**macOS:**
```bash
brew install cmake glfw glew glm
```

**Ubuntu/Debian:**
```bash
sudo apt install cmake libglfw3-dev libglew-dev libglm-dev
```

### Build Steps

```bash
cd wallscene
mkdir build
cd build
cmake ..
make
```

### Run

From the build directory:
```bash
cd bin
./stairwell_scene
```

---

## Controls

| Key/Input | Action |
|-----------|--------|
| W | Move camera forward |
| S | Move camera backward |
| A | Move camera left |
| D | Move camera right |
| Q | Move camera down |
| E | Move camera up |
| Mouse (click + drag) | Look around |
| ESC | Exit program |
| ESC | Exit program |

---

## Author

**Martin Battu**
Computer Graphics Course - Project 4

---

## References

- OpenGL 3.3 Core Profile Specification
- "Learn OpenGL" by Joey de Vries
- GLM Documentation
- GLFW Documentation
