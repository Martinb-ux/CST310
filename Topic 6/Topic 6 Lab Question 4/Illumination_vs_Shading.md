# Topic 6 Lab Question 4: Relationship Between Illumination and Shading

## Relationship Between Illumination and Shading

**Illumination** and **shading** are closely related concepts in computer graphics, but they represent different aspects of light interaction:

### Illumination
- **Definition**: The physical process of light interacting with surfaces in a 3D scene
- **Components**: Includes ambient, diffuse, and specular lighting calculations
- **Purpose**: Determines how much light reaches each point on a surface
- **Factors**: Light source properties, surface material properties, surface orientation (normals)

### Shading
- **Definition**: The method used to apply illumination calculations to render surfaces
- **Purpose**: Determines how to color pixels based on illumination results
- **Methods**: Flat shading, Gouraud shading, Phong shading, etc.

### Key Relationship
- **Illumination provides the data** (light intensity values)
- **Shading applies the data** (determines final pixel colors)
- **Illumination is physics-based**, **shading is algorithmic**
- **Better illumination + better shading = more realistic rendering**

## OpenGL Example

See `illumination_shading_example.cpp` for a complete working OpenGL example demonstrating illumination and shading.

### How the Example Demonstrates the Relationship

#### Illumination Components in the Example:
1. **Light Position**: `light_position` defines where the light source is located
2. **Light Properties**: Ambient, diffuse, and specular components define the light's characteristics
3. **Material Properties**: Define how the surface responds to different types of light

#### Shading Components in the Example:
1. **GL_SMOOTH**: Enables Gouraud shading, which interpolates colors across polygon surfaces
2. **OpenGL Pipeline**: Automatically applies illumination calculations to determine vertex colors
3. **Color Interpolation**: The shading model interpolates between vertex colors to create smooth transitions

#### What You'll See:
- The sphere will appear 3D with proper lighting effects
- Areas facing the light will be brighter (diffuse reflection)
- Specular highlights will appear where the viewing angle aligns with reflection
- The shading will create smooth gradients across the sphere surface

### Compilation Instructions

#### macOS:
```bash
g++ -std=c++11 -Wall -Wextra illumination_shading_example.cpp -o illumination_demo -framework OpenGL -framework GLUT
```

#### Linux:
```bash
g++ -std=c++11 -Wall -Wextra illumination_shading_example.cpp -o illumination_demo -lglut -lGL -lGLU
```

#### Windows:
```bash
g++ -std=c++11 -Wall -Wextra illumination_shading_example.cpp -o illumination_demo.exe -lopengl32 -lglu32 -lfreeglut
```

## Key Takeaways

1. **Illumination calculates** how much light each point receives
2. **Shading determines** how to render those light values on screen
3. **OpenGL combines both** automatically when lighting is enabled
4. **Better materials + proper shading = more realistic results**

This example shows how illumination (light physics) and shading (rendering algorithm) work together to create realistic 3D graphics.
