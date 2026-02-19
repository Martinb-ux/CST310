# Quadrilateral Mesh Shading

This project demonstrates a data structure for representing quadrilateral meshes and implements various shading algorithms in OpenGL.

## Features

### Data Structure
- **Vertex Structure**: Stores position, normal vectors, texture coordinates, and color
- **Quad Structure**: Defines quadrilateral faces using four vertex indices
- **QuadrilateralMesh Class**: Complete mesh management with vertex and face storage

### Shading Algorithms Implemented
1. **Flat Shading**: Each quad uses a single normal for uniform lighting
2. **Smooth Shading**: Vertex normals are interpolated across the surface
3. **Gouraud Shading**: Per-vertex lighting with smooth color interpolation
4. **Phong Shading**: Approximated using enhanced normal interpolation

### Additional Features
- **Height Field Generation**: Creates meshes from mathematical functions
- **Normal Calculation**: Automatic computation of vertex and face normals
- **Color Mapping**: Height-based and slope-based coloring options
- **Interactive Controls**: Real-time camera and shading mode switching

## Building and Running

### Using CMake
```bash
mkdir build
cd build
cmake ..
make
./quadrilateral_mesh_demo
```

### Manual Compilation (macOS/Linux)
```bash
g++ -o quadrilateral_mesh_demo main.cpp quadrilateral_mesh.cpp -lGL -lGLU -lglut
```

## Controls
- **Arrow Keys**: Rotate the view
- **+/-**: Zoom in/out
- **1-4**: Switch between shading modes
- **h**: Toggle between height-based and slope-based coloring
- **Esc**: Exit the application

## Code Structure

### Core Files
- `quadrilateral_mesh.h`: Header file with data structures and class definitions
- `quadrilateral_mesh.cpp`: Implementation of mesh operations and shading algorithms
- `main.cpp`: Main application with OpenGL setup and user interaction
- `CMakeLists.txt`: Build configuration

### Key Classes and Functions

#### Vertex Structure
```cpp
struct Vertex {
    float x, y, z;           // Position
    float nx, ny, nz;         // Normal vector
    float u, v;               // Texture coordinates
    float r, g, b;            // Color
};
```

#### Quad Structure
```cpp
struct Quad {
    int v0, v1, v2, v3;      // Vertex indices
};
```

#### QuadrilateralMesh Class
- `createHeightField()`: Generate mesh from height function
- `calculateNormals()`: Compute vertex normals for lighting
- `renderFlatShading()`: Flat shading implementation
- `renderSmoothShading()`: Smooth shading implementation
- `setColorsFromHeight()`: Height-based color mapping
- `setColorsFromSlope()`: Slope-based color mapping

## Shading Algorithm Details

### Flat Shading
- Uses a single normal per quadrilateral
- Creates a faceted appearance
- Computationally efficient

### Smooth Shading
- Interpolates vertex normals across each quad
- Creates smooth transitions between faces
- Requires normal calculation at vertices

### Gouraud Shading
- Calculates lighting at each vertex
- Interpolates resulting colors across the face
- Good balance between quality and performance

### Phong Shading (Approximation)
- Interpolates normal vectors across the face
- Calculates lighting at each fragment
- Highest quality but requires fragment shaders for true implementation

## Example Height Function
The demo uses a complex height function to create interesting terrain:
```cpp
float heightFunction(float x, float z) {
    return 2.0f * sin(x * 0.5f) * cos(z * 0.5f) + 
           0.5f * sin(x * 2.0f) * sin(z * 2.0f) +
           0.3f * cos(x * 3.0f + z * 2.0f);
}
```

## Technical Notes
- Uses OpenGL fixed-function pipeline for compatibility
- Implements proper normal vector calculation using cross products
- Supports both height-based and slope-based coloring
- Includes coordinate axes for spatial reference
- Features multi-light setup for enhanced visualization
