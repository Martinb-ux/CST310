# Specular Lighting Demo - Assignment Summary

## Overview
This C++/OpenGL program demonstrates the visual impact of different shininess values on specular lighting. The program displays 5 colored cubes, each with a different shininess value, showing how the specular highlight changes based on surface properties.

## Files Created

### 1. SpecularLightingDemo.cpp
- **Main program file** (9,140 bytes)
- Complete, commented C++/OpenGL implementation
- Demonstrates 5 cubes with shininess values: 1, 10, 32, 64, 128
- Features proper Phong lighting model with ambient, diffuse, and specular components
- Interactive camera controls (W/A/S/D/Q/E keys)
- On-screen labels showing shininess values

### 2. README.txt
- **Comprehensive documentation** (5,746 bytes)
- Complete compilation instructions for multiple platforms
- Software requirements and installation guides
- Troubleshooting section
- Educational notes about specular lighting theory

### 3. build.sh
- **Automated build script** (1,074 bytes)
- Cross-platform compilation support
- Detects macOS, Linux, and Windows environments
- Uses appropriate linking flags for each platform

### 4. run_and_capture.sh
- **Screenshot utility script** (1,073 bytes)
- Runs the program and captures screenshots automatically
- Supports macOS screencapture and Linux ImageMagick

### 5. specular_demo
- **Compiled executable** (89,968 bytes)
- Ready-to-run binary for macOS

## Technical Implementation

### Lighting Model
The program implements the Phong lighting model with:
- **Ambient lighting**: Global illumination (0.1, 0.1, 0.1)
- **Diffuse lighting**: Surface color response (0.8, 0.8, 0.8)
- **Specular lighting**: Reflective highlights (1.0, 1.0, 1.0)
- **Light position**: (5.0, 8.0, 5.0, 1.0)

### Material Properties
Each cube has:
- **Ambient component**: 20% of base color
- **Diffuse component**: 100% of base color
- **Specular component**: White (1.0, 1.0, 1.0)
- **Shininess**: Variable (1, 10, 32, 64, 128)

### Cube Colors and Shininess Values
1. **Red cube**: Shininess = 1 (very rough, broad diffuse highlight)
2. **Green cube**: Shininess = 10 (rough surface, moderately broad highlight)
3. **Blue cube**: Shininess = 32 (medium shininess, balanced highlight)
4. **Yellow cube**: Shininess = 64 (shiny surface, concentrated highlight)
5. **Magenta cube**: Shininess = 128 (very shiny, sharp concentrated highlight)

## Key Features Demonstrated

### 1. Specular Lighting Effects
- Shows how shininess affects highlight concentration
- Demonstrates the relationship between surface roughness and light reflection
- Visual comparison of different material properties

### 2. Interactive 3D Graphics
- Real-time rendering with user controls
- Camera movement for viewing from different angles
- Dynamic lighting calculations

### 3. Proper OpenGL Implementation
- Correct normal vectors for lighting calculations
- Material property setup using glMaterialfv()
- Proper depth testing and perspective projection

## Educational Value

### Computer Graphics Concepts Demonstrated:
1. **Phong Reflection Model**: Complete implementation of ambient, diffuse, and specular components
2. **Material Properties**: How surface characteristics affect light interaction
3. **Normal Vectors**: Importance of proper surface orientation for lighting
4. **Real-time Rendering**: Interactive 3D graphics with user control

### Physics Concepts:
1. **Light Reflection**: Specular vs diffuse reflection
2. **Surface Properties**: Roughness and shininess effects
3. **Viewing Geometry**: Relationship between light, surface, and viewer positions

## Compilation and Execution

### Quick Start (macOS):
```bash
./build.sh
./specular_demo
```

### Manual Compilation:
```bash
g++ SpecularLightingDemo.cpp -o specular_demo -framework OpenGL -framework GLUT
```

### Controls:
- **W/S**: Move camera forward/backward
- **A/D**: Move camera left/right
- **Q/E**: Move camera up/down
- **ESC**: Exit program

## Expected Output
The program displays a 3D scene with:
- 5 colored cubes arranged horizontally
- Each cube labeled with its shininess value
- White specular highlights that vary in intensity and concentration
- Ground plane and coordinate axes for reference
- Title text "Specular Lighting - Different Shininess Values"

## Visual Observations
1. **Low shininess (1-10)**: Broad, weak specular highlights that simulate rough surfaces
2. **Medium shininess (32)**: Balanced highlight concentration for typical plastic materials
3. **High shininess (64-128)**: Bright, concentrated highlights simulating shiny or metallic surfaces

## Assignment Requirements Met

✅ **Complete, commented code**: SpecularLightingDemo.cpp with comprehensive comments
✅ **Proper lighting implementation**: Full Phong model with material properties
✅ **Multiple shininess values**: 5 different values clearly demonstrated
✅ **Visual labels**: Each cube labeled with its shininess value
✅ **Compilation instructions**: Detailed README.txt with platform-specific guides
✅ **Screenshot capability**: run_and_capture.sh script for documentation
✅ **Educational value**: Theory explanation and practical demonstration

This implementation provides a comprehensive demonstration of specular lighting effects and serves as an excellent educational tool for understanding computer graphics lighting models.
