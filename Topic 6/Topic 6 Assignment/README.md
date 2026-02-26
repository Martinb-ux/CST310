# Topic 6 - Specular Lighting Demo

This directory contains a C++/OpenGL program demonstrating specular lighting with different shininess values.

## Quick Start

### Build and Run
```bash
make        # Build the program
make run    # Build and run the program
```

### Clean Build
```bash
make clean  # Remove build artifacts
```

## Platform Support

The Makefile automatically detects your platform and uses appropriate libraries:

- **Linux**: Uses `-lglut -lGL -lGLU`
- **macOS**: Uses `-framework OpenGL -framework GLUT`
- **Windows**: Uses `-lopengl32 -lglu32 -lfreeglut`

## Installing Dependencies

### Ubuntu/Debian
```bash
make install-deps
```

### Fedora/CentOS/RHEL
```bash
make install-deps-fedora
```

### Arch Linux
```bash
make install-deps-arch
```

### macOS
```bash
make install-deps-mac
```

## Program Features

The SpecularLightingDemo demonstrates:
- 8 cubes with different shininess values (2, 4, 8, 16, 32, 64, 128, 256)
- Phong reflection model with ambient, diffuse, and specular lighting
- Interactive camera controls
- Real-time specular highlight visualization

### Controls
- **W/S** - Move camera forward/backward
- **A/D** - Move camera left/right
- **Q/E** - Move camera up/down
- **ESC** - Exit program

## Build Options

```bash
make debug    # Build with debug symbols
make release  # Build optimized release version
make help     # Show all available targets
```

## Manual Compilation

### Linux
```bash
g++ -std=c++11 -Wall -Wextra SpecularLightingDemo.cpp -o specular_demo -lglut -lGL -lGLU
```

### macOS
```bash
g++ -std=c++11 -Wall -Wextra SpecularLightingDemo.cpp -o specular_demo -framework OpenGL -framework GLUT
```

### Windows
```bash
g++ -std=c++11 -Wall -Wextra SpecularLightingDemo.cpp -o specular_demo.exe -lopengl32 -lglu32 -lfreeglut
```
