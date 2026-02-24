#!/bin/bash

# Build script for SpecularLightingDemo
# This script detects the platform and uses appropriate compilation flags

echo "Building SpecularLightingDemo..."

# Detect operating system
OS=$(uname -s)

case "$OS" in
    Darwin*)
        echo "Detected macOS, using framework linking..."
        g++ SpecularLightingDemo.cpp -o specular_demo -framework OpenGL -framework GLUT
        ;;
    Linux*)
        echo "Detected Linux, using library linking..."
        g++ SpecularLightingDemo.cpp -o specular_demo -lglut -lGL -lGLU
        ;;
    CYGWIN*|MINGW*|MSYS*)
        echo "Detected Windows, using MinGW linking..."
        g++ SpecularLightingDemo.cpp -o specular_demo.exe -lopengl32 -lglu32 -lfreeglut
        ;;
    *)
        echo "Unknown operating system: $OS"
        echo "Trying generic compilation..."
        g++ SpecularLightingDemo.cpp -o specular_demo -lglut -lGL -lGLU
        ;;
esac

if [ $? -eq 0 ]; then
    echo "Build successful!"
    echo "Run the program with: ./specular_demo"
else
    echo "Build failed. Check error messages above."
fi
