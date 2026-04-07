# Project Folder Structure

This document explains the organized folder structure for the OpenGL 3D Scene project.

## Directory Layout

```
Topic 9/
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
│   ├── cube.vs            # Cube vertex shader
│   ├── cube.frag          # Cube fragment shader
│   ├── cylinder.vs        # Cylinder vertex shader
│   ├── cylinder.frag      # Cylinder fragment shader
│   ├── sphere.vs          # Sphere vertex shader
│   └── sphere.frag        # Sphere fragment shader
├── models/                 # 3D model files
│   ├── cylinder.obj       # Cylinder 3D model
│   ├── cylinder.mtl       # Cylinder material file
│   ├── sphere.obj         # Sphere 3D model
│   └── sphere.mtl         # Sphere material file
├── docs/                   # Documentation
│   ├── PROJECT_SUMMARY.txt # Complete project overview
│   ├── CHECKERBOARD_CODE.txt # Checkerboard rendering code
│   ├── FOLDER_STRUCTURE.md # This file
│   └── README.md          # Original README (moved here)
├── screenshot/             # Screenshots
│   └── (screenshot files)
├── Makefile               # Build configuration
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
All vertex and fragment shaders are organized by object:
- **checkerboard.**: Shaders for the 8x8 checkerboard floor
- **cube.**: Shaders for the central red cube
- **cylinder.**: Shaders for the green cylinder model
- **sphere.**: Shaders for the blue sphere model

### `/models/` - 3D Models
OBJ and MTL files for the 3D models used in the scene:
- **cylinder.obj/.mtl**: Green cylinder model with materials
- **sphere.obj/.mtl**: Blue sphere model with materials

### `/docs/` - Documentation
All project documentation and code examples:
- **PROJECT_SUMMARY.txt**: Complete project overview with controls and requirements
- **CHECKERBOARD_CODE.txt**: Detailed checkerboard rendering code with comments
- **FOLDER_STRUCTURE.md**: This folder structure documentation
- **README.md**: Original README file with build instructions

## Benefits of This Organization

1. **Clean Separation**: Related files are grouped together logically
2. **Easy Navigation**: Clear directory structure makes finding files intuitive
3. **Scalability**: Easy to add new shaders, models, or source files
4. **Maintainability**: Organized structure simplifies project maintenance
5. **Professional**: Follows common software development practices

## Building and Running

The Makefile has been updated to work with this structure:

```bash
make          # Build the project
make execute  # Build and run
make clean    # Clean build artifacts
```

All relative paths in the code have been updated to work with the new directory structure. The application can be run from the project root directory as before.
