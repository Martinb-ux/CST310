# Topic 10 - Advanced Shading Techniques Project Structure

This document explains the organized folder structure for the OpenGL Advanced Shading project.

## Directory Layout

```
Topic 10/
├── src/                    # Source code files
│   └── main.cpp           # Main application entry point
├── include/                # Header files
│   ├── Camera.h           # Camera class with movement controls
│   ├── Model.h            # 3D model loading class
│   ├── Mesh.h             # Mesh rendering class
│   ├── shader.h           # Shader program wrapper
│   └── stb_image.h        # Image loading library
├── shaders/                # OpenGL shader files
│   ├── checkerboard.vs    # Checkerboard vertex shader
│   ├── checkerboard.frag  # Checkerboard fragment shader
│   ├── cube.vs            # Cube vertex shader (parallax mapping)
│   ├── cube.frag          # Cube fragment shader (parallax mapping)
│   ├── cylinder.vs        # Cylinder vertex shader (bump mapping)
│   ├── cylinder.frag      # Cylinder fragment shader (bump mapping)
│   ├── sphere.vs          # Sphere vertex shader (environment mapping)
│   └── sphere.frag        # Sphere fragment shader (environment mapping)
├── models/                 # 3D model files
│   ├── cylinder.obj       # Cylinder 3D model
│   ├── cylinder.mtl       # Cylinder material file
│   ├── sphere.obj         # Sphere 3D model
│   └── sphere.mtl         # Sphere material file
├── pictures/               # Texture files for advanced shading
│   ├── Bump-Map.jpg       # Height/Normal map for cube and cylinder
│   ├── Bump-Picture.jpg   # Diffuse texture for cube and cylinder
│   └── Yokohama2/         # Cube map for environment mapping
│       ├── posx.jpg, negx.jpg, posy.jpg, negy.jpg, posz.jpg, negz.jpg
├── docs/                   # Documentation
│   ├── PROJECT_SUMMARY.txt # Complete project overview
│   ├── CHECKERBOARD_CODE.txt # Checkerboard rendering code
│   ├── FOLDER_STRUCTURE.md # This file
│   └── README.md          # Original README (moved here)
├── screenshot/             # Screenshots
│   └── (screenshot files)
├── video/                  # Video recordings
│   └── (video files)
├── Makefile               # Build configuration
├── instructions.txt       # Build and run instructions for Topic 10
└── run                    # Compiled executable
```

## File Organization

### `/src/` - Source Code
Contains the main C++ source file for the application.

### `/include/` - Header Files
All header files are grouped here for easy access and clean organization:
- **Camera.h**: Camera class with 6DOF movement and rotation controls
- **Model.h**: 3D model loading using Assimp with stb_image texture support
- **Mesh.h**: Mesh rendering functionality
- **shader.h**: OpenGL shader program wrapper class
- **stb_image.h**: Header-only image loading library

### `/shaders/` - OpenGL Shaders
All vertex and fragment shaders are organized by object with advanced shading:
- **checkerboard.**: Shaders for the 8x8 checkerboard floor (basic lighting)
- **cube.**: Shaders for the central cube with Environment Mapping
- **cylinder.**: Shaders for the cylinder model with Bump Mapping
- **sphere.**: Shaders for the sphere model with Parallax Occlusion Mapping

### `/models/` - 3D Models
OBJ and MTL files for the 3D models used in the scene:
- **cylinder.obj/.mtl**: Cylinder model for bump mapping demonstration
- **sphere.obj/.mtl**: Sphere model for parallax mapping demonstration

### `/pictures/` - Texture Files
All texture files for advanced shading techniques:
- **Bump-Map.jpg**: Height map (sphere) and normal map (cylinder)
- **Bump-Picture.jpg**: Diffuse texture for sphere and cylinder
- **Yokohama2/**: Cube map images for environment mapping
  - **posx.jpg, negx.jpg, posy.jpg, negy.jpg, posz.jpg, negz.jpg**: Six cube map faces

### `/docs/` - Documentation
All project documentation and code examples:
- **PROJECT_SUMMARY.txt**: Complete Topic 10 project overview with advanced shading
- **CHECKERBOARD_CODE.txt**: Detailed checkerboard rendering code with comments
- **FOLDER_STRUCTURE.md**: This folder structure documentation
- **README.md**: Original README file with build instructions

## Benefits of This Organization

1. **Clean Separation**: Related files are grouped together logically
2. **Easy Navigation**: Clear directory structure makes finding files intuitive
3. **Scalability**: Easy to add new shaders, models, or texture files
4. **Maintainability**: Organized structure simplifies project maintenance
5. **Professional**: Follows common software development practices
6. **Advanced Shading Support**: Dedicated directories for textures and specialized shaders

## Building and Running

The Makefile has been updated to work with this structure:

```bash
make          # Build the project
make execute  # Build and run
make clean    # Clean build artifacts
```

All relative paths in the code have been updated to work with the new directory structure. The application can be run from the project root directory as before.
