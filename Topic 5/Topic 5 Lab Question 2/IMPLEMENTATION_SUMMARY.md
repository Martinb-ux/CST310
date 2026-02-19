# Quadrilateral Mesh Data Structure and Shading Implementation

## Overview
This implementation provides a complete solution for representing quadrilateral meshes and applying various shading algorithms using OpenGL.

## Data Structure Design

### Vertex Structure
```cpp
struct Vertex {
    float x, y, z;           // 3D position
    float nx, ny, nz;         // Normal vector for lighting calculations
    float u, v;               // Texture coordinates (for future texturing)
    float r, g, b;            // RGB color values
};
```

### Quad Structure
```cpp
struct Quad {
    int v0, v1, v2, v3;      // Indices into the vertex array
};
```

### QuadrilateralMesh Class
The main class that manages the mesh data and provides rendering capabilities:

**Key Features:**
- Dynamic vertex and quad storage using std::vector
- Automatic normal calculation for lighting
- Multiple shading algorithm implementations
- Height field generation from mathematical functions
- Color mapping based on height or slope

## Shading Algorithms Implemented

### 1. Flat Shading
- **Method**: Uses a single normal per quadrilateral
- **Implementation**: Calculates face normal and applies it to all vertices
- **Visual Effect**: Creates a faceted, polygonal appearance
- **Performance**: Most computationally efficient

### 2. Smooth Shading
- **Method**: Interpolates vertex normals across the surface
- **Implementation**: Calculates normals at each vertex and uses OpenGL's smooth shading
- **Visual Effect**: Creates smooth transitions between faces
- **Performance**: Moderate computational cost

### 3. Gouraud Shading
- **Method**: Per-vertex lighting with color interpolation
- **Implementation**: Uses OpenGL's lighting model with smooth shading
- **Visual Effect**: Smooth color gradients across surfaces
- **Performance**: Good balance between quality and speed

### 4. Phong Shading (Approximation)
- **Method**: Normal interpolation with per-pixel lighting (approximated)
- **Implementation**: Enhanced smooth shading with better normal calculation
- **Visual Effect**: Highest quality smooth shading
- **Note**: True Phong shading requires fragment shaders

## Key Algorithms

### Normal Calculation
```cpp
void calculateTriangleNormal(const Vertex& v1, const Vertex& v2, const Vertex& v3, 
                           float& nx, float& ny, float& nz) {
    // Calculate edge vectors
    float edge1x = v2.x - v1.x, edge1y = v2.y - v1.y, edge1z = v2.z - v1.z;
    float edge2x = v3.x - v1.x, edge2y = v3.y - v1.y, edge2z = v3.z - v1.z;
    
    // Cross product for normal
    nx = edge1y * edge2z - edge1z * edge2y;
    ny = edge1z * edge2x - edge1x * edge2z;
    nz = edge1x * edge2y - edge1y * edge2x;
    
    // Normalize
    float length = sqrt(nx*nx + ny*ny + nz*nz);
    if (length > 0.0001f) {
        nx /= length; ny /= length; nz /= length;
    }
}
```

### Height Field Generation
The mesh can be generated from any mathematical function:
```cpp
float heightFunction(float x, float z) {
    return 2.0f * sin(x * 0.5f) * cos(z * 0.5f) + 
           0.5f * sin(x * 2.0f) * sin(z * 2.0f) +
           0.3f * cos(x * 3.0f + z * 2.0f);
}
```

### Color Mapping
Two coloring strategies are implemented:

1. **Height-based**: Maps elevation to color gradient (blue=low, red=high)
2. **Slope-based**: Maps surface steepness to color (green=flat, red=steep)

## Usage Instructions

### Building
```bash
make
```

### Running
```bash
./quadrilateral_mesh_demo
```

### Controls
- **Arrow Keys**: Rotate view
- **+/-**: Zoom in/out
- **1-4**: Switch shading modes
- **h**: Toggle height/slope coloring
- **Esc**: Exit

## Technical Details

### Memory Management
- Uses RAII with std::vector for automatic memory management
- No manual memory allocation/deallocation required
- Efficient storage with contiguous memory layout

### Performance Considerations
- Normal calculation is done once during mesh creation
- Rendering uses immediate mode for compatibility
- Quad decomposition into triangles for normal calculation

### Extensibility
The design allows for easy extension:
- Additional shading algorithms
- Different mesh generation methods
- Texture mapping support
- Level-of-detail systems

## File Structure
- `quadrilateral_mesh.h`: Class definitions and structures
- `quadrilateral_mesh.cpp`: Implementation of mesh operations
- `main.cpp`: Demo application with OpenGL setup
- `Makefile`: Build configuration for macOS
- `CMakeLists.txt`: Cross-platform build configuration

## Educational Value
This implementation demonstrates:
1. Data structure design for 3D graphics
2. Normal vector mathematics
3. Various shading techniques
4. OpenGL integration
5. Interactive 3D visualization

The code is well-documented and modular, making it suitable for learning and modification.
