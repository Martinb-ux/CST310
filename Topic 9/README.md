# Topic 9 - OpenGL 3D Scene

An OpenGL application that renders a 3D scene with a checkerboard floor, cube, cylinder, and sphere using modern OpenGL techniques.

## Features

- **3D Objects**: Renders a cube, cylinder, and sphere from OBJ files
- **Checkerboard Floor**: 8x8 checkerboard pattern with alternating colors
- **Camera Controls**: Full camera movement with keyboard controls
- **Lighting**: Basic lighting system with configurable light position
- **Shaders**: Multiple shader programs for different objects
- **Texture Loading**: Uses stb_image for texture loading (SOIL replacement)

## Dependencies

### macOS (Homebrew)
```bash
brew install glfw glew assimp glm
```

### Required Libraries
- **GLFW**: Window management and input handling
- **GLEW**: OpenGL extension loading
- **Assimp**: 3D model loading (OBJ files)
- **GLM**: Mathematics library for OpenGL
- **stb_image**: Image loading library (included)

## Building

### Using Makefile (Recommended)
```bash
make          # Build the project
make execute  # Build and run the project
```

### Manual Compilation
```bash
g++ main.cpp -o run -L/opt/homebrew/lib -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -lGLEW -lassimp
```

## Running

```bash
./run
```

## Controls

### Camera Movement
- **Right Arrow**: Slide camera 1 unit in the positive X direction
- **Left Arrow**: Slide camera 1 unit in the negative X direction  
- **Up Arrow**: Slide camera 1 unit in the positive Y direction
- **Down Arrow**: Slide camera 1 unit in the negative Y direction
- **Shift + Up Arrow**: Slide camera 1 unit in the positive Z ("in") direction
- **Shift + Down Arrow**: Slide camera 1 unit in the negative Z ("out") direction
- **Command + Up Arrow**: Change camera pitch by -2 degrees
- **Command + Down Arrow**: Change camera pitch by 2 degrees
- **Command + Right Arrow**: Change camera yaw by 2 degrees
- **Command + Left Arrow**: Change camera yaw by -2 degrees
- **Shift + Comma (<)**: Change camera roll by 2 degrees
- **Shift + Period (>)**: Change camera roll by -2 degrees
- **R**: Reset to the default position and orientation

### Exit
- **ESC**: Close the application

### Notes
- **Command key** refers to the ⌘ key on Mac keyboards
- For non-Mac systems, use the Control key instead

## File Structure

```
Topic 9/
├── main.cpp              # Main application entry point
├── shader.h              # Shader class wrapper
├── Camera.h              # Camera implementation
├── Model.h               # 3D model loading class
├── Mesh.h                # Mesh rendering class
├── stb_image.h           # Image loading library
├── *.vs                  # Vertex shader files
├── *.frag                # Fragment shader files
├── *.obj                 # 3D model files
├── *.mtl                 # Material files
├── Makefile              # Build configuration
└── README.md             # This file
```

## Shaders

- **checkerboard.vs/frag**: Checkerboard floor shader
- **cube.vs/frag**: Cube rendering shader
- **cylinder.vs/frag**: Cylinder rendering shader
- **sphere.vs/frag**: Sphere rendering shader

## Notes

- This project uses stb_image instead of SOIL for better cross-platform compatibility
- The application creates an 800x600 window
- All objects are rendered with basic Phong lighting
- The checkerboard floor extends from (-4, -9) to (4, -1) in world coordinates

## Troubleshooting

### Compilation Issues
- Ensure all dependencies are installed via Homebrew
- Check that Homebrew libraries are in `/opt/homebrew/lib` (Apple Silicon) or `/usr/local/lib` (Intel)
- Make sure Xcode Command Line Tools are installed

### Runtime Issues
- The application requires OpenGL 3.3 core profile
- Ensure your graphics drivers are up to date
- Check that OBJ and MTL files are present in the same directory as the executable
