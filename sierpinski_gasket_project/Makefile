# Makefile for Sierpinski Gasket Project
# Supports macOS and Linux platforms

# Detect operating system
UNAME_S := $(shell uname -s)

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -O2

# Output executables
TARGET_2D_RANDOM = gasket_2d_random
TARGET_2D_SUBDIV = gasket_2d_subdivision
TARGET_3D_TETRA = gasket_3d_tetrahedron

# Source files
SRC_2D_RANDOM = gasket_2d_random.cpp
SRC_2D_SUBDIV = gasket_2d_subdivision.cpp
SRC_3D_TETRA = gasket_3d_tetrahedron.cpp

# Platform-specific linker flags
ifeq ($(UNAME_S),Darwin)
    # macOS - use frameworks
    LDFLAGS = -framework OpenGL -framework GLUT
else
    # Linux - use libraries
    LDFLAGS = -lGL -lGLU -lglut
endif

# Default target - build all
.PHONY: all
all: $(TARGET_2D_RANDOM) $(TARGET_2D_SUBDIV) $(TARGET_3D_TETRA)
	@echo "============================================"
	@echo "All programs compiled successfully!"
	@echo "============================================"
	@echo "Run with:"
	@echo "  ./$(TARGET_2D_RANDOM)"
	@echo "  ./$(TARGET_2D_SUBDIV)"
	@echo "  ./$(TARGET_3D_TETRA)"
	@echo "============================================"

# Build 2D Random Point Method
$(TARGET_2D_RANDOM): $(SRC_2D_RANDOM)
	@echo "Compiling 2D Random Point Method..."
	$(CXX) $(CXXFLAGS) -o $(TARGET_2D_RANDOM) $(SRC_2D_RANDOM) $(LDFLAGS)

# Build 2D Subdivision Method
$(TARGET_2D_SUBDIV): $(SRC_2D_SUBDIV)
	@echo "Compiling 2D Subdivision Method..."
	$(CXX) $(CXXFLAGS) -o $(TARGET_2D_SUBDIV) $(SRC_2D_SUBDIV) $(LDFLAGS)

# Build 3D Tetrahedron Method
$(TARGET_3D_TETRA): $(SRC_3D_TETRA)
	@echo "Compiling 3D Tetrahedron Method..."
	$(CXX) $(CXXFLAGS) -o $(TARGET_3D_TETRA) $(SRC_3D_TETRA) $(LDFLAGS)

# Clean up executables
.PHONY: clean
clean:
	@echo "Cleaning up..."
	rm -f $(TARGET_2D_RANDOM) $(TARGET_2D_SUBDIV) $(TARGET_3D_TETRA)
	rm -f *.o
	@echo "Clean complete!"

# Run 2D Random
.PHONY: run-random
run-random: $(TARGET_2D_RANDOM)
	@echo "Running 2D Random Point Method..."
	./$(TARGET_2D_RANDOM)

# Run 2D Subdivision
.PHONY: run-subdiv
run-subdiv: $(TARGET_2D_SUBDIV)
	@echo "Running 2D Subdivision Method..."
	./$(TARGET_2D_SUBDIV)

# Run 3D Tetrahedron
.PHONY: run-3d
run-3d: $(TARGET_3D_TETRA)
	@echo "Running 3D Tetrahedron Method..."
	./$(TARGET_3D_TETRA)

# Help target
.PHONY: help
help:
	@echo "Sierpinski Gasket Project - Makefile"
	@echo "===================================="
	@echo "Available targets:"
	@echo "  make          - Build all three programs"
	@echo "  make clean    - Remove all executables"
	@echo "  make run-random  - Build and run 2D Random Method"
	@echo "  make run-subdiv  - Build and run 2D Subdivision"
	@echo "  make run-3d      - Build and run 3D Tetrahedron"
	@echo "  make help     - Show this help message"
	@echo ""
	@echo "Individual builds:"
	@echo "  make $(TARGET_2D_RANDOM)"
	@echo "  make $(TARGET_2D_SUBDIV)"
	@echo "  make $(TARGET_3D_TETRA)"
