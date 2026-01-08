#!/bin/bash

# Quick Build and Run Script for Sierpinski Gasket Project
# This script detects the operating system and compiles/runs the programs

echo "========================================"
echo "Sierpinski Gasket Project - Quick Start"
echo "========================================"
echo ""

# Detect OS
OS="$(uname -s)"
case "${OS}" in
    Linux*)     MACHINE=Linux;;
    Darwin*)    MACHINE=Mac;;
    CYGWIN*)    MACHINE=Cygwin;;
    MINGW*)     MACHINE=MinGW;;
    *)          MACHINE="UNKNOWN:${OS}"
esac

echo "Detected OS: $MACHINE"
echo ""

# Set compiler flags based on OS
if [ "$MACHINE" == "Mac" ]; then
    LDFLAGS="-framework OpenGL -framework GLUT"
elif [ "$MACHINE" == "Linux" ]; then
    LDFLAGS="-lGL -lGLU -lglut"
else
    echo "Warning: Unsupported OS. Trying Linux flags..."
    LDFLAGS="-lGL -lGLU -lglut"
fi

# Function to compile a program
compile_program() {
    local source=$1
    local output=$2
    echo "Compiling $source..."
    g++ -o "$output" "$source" $LDFLAGS
    if [ $? -eq 0 ]; then
        echo "✓ $output compiled successfully"
        return 0
    else
        echo "✗ Failed to compile $output"
        return 1
    fi
}

# Compile all programs
echo "Step 1: Compiling all programs..."
echo "--------------------------------"
compile_program "gasket_2d_random.cpp" "gasket_2d_random"
compile_program "gasket_2d_subdivision.cpp" "gasket_2d_subdivision"
compile_program "gasket_3d_tetrahedron.cpp" "gasket_3d_tetrahedron"
echo ""

# Check if all compiled successfully
if [ -f "gasket_2d_random" ] && [ -f "gasket_2d_subdivision" ] && [ -f "gasket_3d_tetrahedron" ]; then
    echo "✓ All programs compiled successfully!"
    echo ""
    
    # Interactive menu
    while true; do
        echo "========================================"
        echo "Select a program to run:"
        echo "========================================"
        echo "1) 2D Random Point Method"
        echo "2) 2D Subdivision Method"
        echo "3) 3D Tetrahedron Method"
        echo "4) Run all programs sequentially"
        echo "5) Exit"
        echo ""
        read -p "Enter your choice (1-5): " choice
        
        case $choice in
            1)
                echo ""
                echo "Launching 2D Random Point Method..."
                echo "Controls: +/- to adjust points, R to reset, ESC to exit"
                ./gasket_2d_random
                ;;
            2)
                echo ""
                echo "Launching 2D Subdivision Method..."
                echo "Controls: +/- to adjust depth, SPACE for wireframe, ESC to exit"
                ./gasket_2d_subdivision
                ;;
            3)
                echo ""
                echo "Launching 3D Tetrahedron Method..."
                echo "Controls: Arrows to rotate, SPACE for animation, W for wireframe, ESC to exit"
                ./gasket_3d_tetrahedron
                ;;
            4)
                echo ""
                echo "Running all programs sequentially..."
                echo "Close each window to proceed to the next."
                echo ""
                echo "Starting 2D Random Point Method..."
                ./gasket_2d_random
                echo "Starting 2D Subdivision Method..."
                ./gasket_2d_subdivision
                echo "Starting 3D Tetrahedron Method..."
                ./gasket_3d_tetrahedron
                ;;
            5)
                echo ""
                echo "Exiting. Thank you!"
                exit 0
                ;;
            *)
                echo ""
                echo "Invalid choice. Please enter 1-5."
                ;;
        esac
        echo ""
    done
else
    echo "✗ Some programs failed to compile."
    echo "Please check the error messages above."
    echo "Make sure you have OpenGL and GLUT installed."
    echo ""
    echo "Installation help:"
    if [ "$MACHINE" == "Mac" ]; then
        echo "  macOS: xcode-select --install"
        echo "  or: brew install freeglut"
    elif [ "$MACHINE" == "Linux" ]; then
        echo "  Ubuntu/Debian: sudo apt-get install freeglut3-dev"
        echo "  Fedora: sudo dnf install freeglut-devel"
    fi
    exit 1
fi
