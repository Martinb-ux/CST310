# Makefile for Specular Lighting Demo - Topic 6
# C++/OpenGL program demonstrating specular lighting with different shininess values

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
TARGET = specular_demo
SOURCE = SpecularLightingDemo.cpp

# Platform-specific libraries
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    # macOS
    LIBS = -framework OpenGL -framework GLUT
else ifeq ($(UNAME_S),Linux)
    # Linux
	LIBS = -lGLEW -lglut -lGL -lGLU
else ifeq ($(OS),Windows_NT)
    # Windows (MinGW/Cygwin)
    LIBS = -lopengl32 -lglu32 -lfreeglut
else
    # Default to Linux-style libraries
	LIBS = -lGLEW -lglut -lGL -lGLU
endif

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $(TARGET) $(LIBS)

# Clean build artifacts
clean:
	rm -f $(TARGET) $(TARGET).exe

# Run the program
run: $(TARGET)
	./$(TARGET)

# Install dependencies (Ubuntu/Debian)
install-deps:
	@echo "Installing dependencies for Ubuntu/Debian..."
	sudo apt-get update
	sudo apt-get install build-essential libgl1-mesa-dev libglu1-mesa-dev libglut-dev libglew-dev

# Install dependencies (Fedora/CentOS/RHEL)
install-deps-fedora:
	@echo "Installing dependencies for Fedora/CentOS/RHEL..."
	sudo dnf install gcc-c++ mesa-libGL-devel mesa-libGLU-devel freeglut-devel

# Install dependencies (Arch Linux)
install-deps-arch:
	@echo "Installing dependencies for Arch Linux..."
	sudo pacman -S base-devel mesa glu freeglut

# Install dependencies (macOS with Homebrew)
install-deps-mac:
	@echo "Installing dependencies for macOS..."
	brew install freeglut

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

# Release build
release: CXXFLAGS += -O2 -DNDEBUG
release: $(TARGET)

# Help target
help:
	@echo "Available targets:"
	@echo "  all              - Build the specular lighting demo (default)"
	@echo "  clean            - Remove build artifacts"
	@echo "  run              - Build and run the program"
	@echo "  debug            - Build with debug symbols"
	@echo "  release          - Build optimized release version"
	@echo "  install-deps     - Install dependencies (Ubuntu/Debian)"
	@echo "  install-deps-fedora - Install dependencies (Fedora/CentOS/RHEL)"
	@echo "  install-deps-arch   - Install dependencies (Arch Linux)"
	@echo "  install-deps-mac    - Install dependencies (macOS)"
	@echo "  help             - Show this help message"
	@echo ""
	@echo "Platform detected: $(UNAME_S)"
	@echo "Libraries: $(LIBS)"

.PHONY: all clean run install-deps install-deps-fedora install-deps-arch install-deps-mac debug release help
