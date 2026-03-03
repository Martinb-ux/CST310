# Makefile for Project7Files OpenGL Programs
# Supports compilation on macOS and Linux

# Detect operating system
UNAME_S := $(shell uname -s)

# Compiler and flags based on OS
ifeq ($(UNAME_S),Darwin)
    # macOS
    CXX = clang++
    CXXFLAGS = -std=c++11 -Wall -Wextra
    LDFLAGS = -framework OpenGL -framework GLUT
else
    # Linux
    CXX = g++
    CXXFLAGS = -std=c++11 -Wall -Wextra
    LDFLAGS = -lGL -lGLU -lglut
endif

# Source files
CHECKERED_SRC = CheckeredTriangles.cpp
COLORCUBE_SRC = ColorCubeFlyby.cpp

# Executable names
CHECKERED_EXEC = triangles
COLORCUBE_EXEC = flyby

# Default target
all: $(CHECKERED_EXEC) $(COLORCUBE_EXEC)

# Build CheckeredTriangles
$(CHECKERED_EXEC): $(CHECKERED_SRC)
	@echo "Building CheckeredTriangles..."
	$(CXX) $(CXXFLAGS) $(CHECKERED_SRC) -o $(CHECKERED_EXEC) $(LDFLAGS)
	@echo "Built $(CHECKERED_EXEC) successfully!"

# Build ColorCubeFlyby
$(COLORCUBE_EXEC): $(COLORCUBE_SRC)
	@echo "Building ColorCubeFlyby..."
	$(CXX) $(CXXFLAGS) $(COLORCUBE_SRC) -o $(COLORCUBE_EXEC) $(LDFLAGS)
	@echo "Built $(COLORCUBE_EXEC) successfully!"

# Run CheckeredTriangles
run-checkered: $(CHECKERED_EXEC)
	@echo "Running CheckeredTriangles..."
	./$(CHECKERED_EXEC)

# Run ColorCubeFlyby
run-flyby: $(COLORCUBE_EXEC)
	@echo "Running ColorCubeFlyby..."
	./$(COLORCUBE_EXEC)

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -f $(CHECKERED_EXEC) $(COLORCUBE_EXEC)
	@echo "Clean complete!"

# Install dependencies (platform-specific)
install-deps:
	@echo "Installing dependencies for $(UNAME_S)..."
ifeq ($(UNAME_S),Darwin)
	@echo "On macOS, ensure Xcode Command Line Tools are installed:"
	@echo "  xcode-select --install"
else
	@echo "On Linux, install OpenGL/GLUT development packages:"
	@echo "  Ubuntu/Debian: sudo apt-get install freeglut3-dev libgl1-mesa-dev"
	@echo "  Fedora/RHEL:   sudo dnf install freeglut-devel mesa-libGL-devel"
	@echo "  Arch Linux:    sudo pacman -S freeglut mesa"
endif

# Help target
help:
	@echo "Available targets:"
	@echo "  all           - Build both programs"
	@echo "  triangles     - Build CheckeredTriangles only"
	@echo "  flyby         - Build ColorCubeFlyby only"
	@echo "  run-checkered - Build and run CheckeredTriangles"
	@echo "  run-flyby     - Build and run ColorCubeFlyby"
	@echo "  clean         - Remove executable files"
	@echo "  install-deps  - Show dependency installation instructions"
	@echo "  help          - Show this help message"

# Phony targets
.PHONY: all run-checkered run-flyby clean install-deps help
