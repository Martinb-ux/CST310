# Project 7 Files - OpenGL Graphics Programs

This directory contains two OpenGL graphics programs demonstrating various computer graphics concepts including texture mapping, 3D transformations, lighting, and animation.

## Programs Overview

### 1. CheckeredTriangles.cpp
A program that displays three textured triangles with checkered patterns, featuring rotation animation and interactive controls.

**Features:**
- 2x2 checkered texture mapping (red and yellow pattern)
- Continuous rotation animation
- Interactive keyboard controls for movement and zoom
- Smooth double-buffered rendering at ~60 FPS

### 2. ColorCubeFlyby.cpp
An advanced 3D scene featuring multiple colored cubes with lighting, camera flyby animation, and physics-based bouncing between walls.

**Features:**
- Multiple colored cubes with different materials and lighting properties
- Automatic camera flyby with trigonometric motion paths
- Physics simulation with cubes bouncing between vertical walls
- OpenGL lighting with ambient, diffuse, and specular components
- Depth buffering for proper 3D rendering
- Interactive scene controls

## Software Requirements

### Required Libraries
- **OpenGL** (OpenGL 1.1 or higher)
- **GLUT** (OpenGL Utility Toolkit)
- **C++ Standard Library**

### Platform-Specific Requirements

#### macOS
- OpenGL and GLUT are included with Xcode Command Line Tools
- Note: OpenGL/GLUT are deprecated on macOS but functional for academic purposes
- The code includes `GL_SILENCE_DEPRECATION` to suppress warnings

#### Linux
- Install development packages:
  ```bash
  # Ubuntu/Debian
  sudo apt-get install freeglut3-dev libgl1-mesa-dev
  
  # Fedora/RHEL
  sudo dnf install freeglut-devel mesa-libGL-devel
  
  # Arch Linux
  sudo pacman -S freeglut mesa
  ```

#### Windows
- Install GLUT for Windows (freeglut recommended)
- Visual Studio with C++ development tools
- OpenGL drivers (typically included with graphics drivers)

## Compilation Instructions

### Using clang++ (macOS)
```bash
# Compile CheckeredTriangles
clang++ CheckeredTriangles.cpp -o triangles -framework OpenGL -framework GLUT
./triangles

# Compile ColorCubeFlyby
clang++ ColorCubeFlyby.cpp -o flyby -framework OpenGL -framework GLUT
./flyby
```

### Using g++ (Linux)
```bash
# Compile CheckeredTriangles
g++ CheckeredTriangles.cpp -o triangles -lGL -lGLU -lglut
./triangles

# Compile ColorCubeFlyby  
g++ ColorCubeFlyby.cpp -o flyby -lGL -lGLU -lglut
./flyby
```

### Using Make (Cross-platform)
```bash
# Build both programs
make

# Build individual programs
make triangles    # Build CheckeredTriangles
make flyby        # Build ColorCubeFlyby

# Build and run
make run-checkered  # Build and run CheckeredTriangles
make run-flyby      # Build and run ColorCubeFlyby

# Clean build artifacts
make clean

# Show help
make help

# Install dependencies (shows instructions)
make install-deps
```

### Using Visual Studio (Windows)
1. Create new C++ Console Application project
2. Add the .cpp file to the project
3. Configure project properties to link OpenGL libraries:
   - Additional Dependencies: opengl32.lib, glu32.lib, glut32.lib
4. Build and run

## Running the Programs

### CheckeredTriangles
```bash
./CheckeredTriangles
```

**Controls:**
- `p` - Pause spinning animation
- `c` - Continue spinning animation  
- `u`/`d` - Move triangles up/down
- `l`/`r` (or `L`/`R`) - Move triangles left/right
- `+`/`-` - Zoom in/out
- `Esc` - Quit program

### ColorCubeFlyby
```bash
./ColorCubeFlyby
```

**Controls:**
- `r` - Toggle scene rotation on/off
- `s` - Stop (pause camera flyby and cube motion)
- `c` - Continue (resume animation)
- `u`/`d` - Move entire scene up/down
- `+`/`-` - Zoom in/out
- `Esc` - Quit program

## Technical Details

### CheckeredTriangles
- Uses `GL_TEXTURE_2D` for texture mapping
- Implements timer-based animation at 16ms intervals (~60 FPS)
- Texture coordinates demonstrate texture wrapping effects
- 2D orthographic projection with 3D perspective camera

### ColorCubeFlyby
- Implements Phong lighting model with multiple material properties
- Uses depth buffering (`GL_DEPTH_TEST`) for proper 3D rendering
- Camera flyby uses trigonometric functions for smooth motion paths
- Physics simulation includes collision detection and response
- Multiple cubes with varying material properties (brightness, shininess)

## Troubleshooting

### Common Issues
1. **GLUT not found**: Install GLUT development package for your platform
2. **OpenGL errors**: Update graphics drivers
3. **Compilation errors**: Ensure all required libraries are linked
4. **Window doesn't appear**: Check that graphics drivers support OpenGL

### macOS Specific
- If you see deprecation warnings, these are normal and expected
- The `GL_SILENCE_DEPRECATION` macro suppresses these warnings
- Programs should run normally despite the warnings

## Educational Value

These programs demonstrate key computer graphics concepts:
- Texture mapping and coordinate systems
- 3D transformations and matrix operations
- Lighting models and material properties
- Animation and timer-based updates
- User interaction and event handling
- Depth buffering and 3D rendering techniques

## File Structure
```
Project7Files/
├── README.md                 # This file
├── Makefile                  # Cross-platform build system
├── CheckeredTriangles.cpp    # Textured triangles program
└── ColorCubeFlyby.cpp       # 3D cube flyby program
```

Both programs are standalone and can be compiled and run independently.
