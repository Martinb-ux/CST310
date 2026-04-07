# Makefile for OpenGL 3D Scene Project
# Author:ki Austinen
# Date: March 2026

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2

# Target executable
TARGET = run

# Source files
SOURCES = src/main.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Library paths and includes
# Check if we're on Apple Silicon or Intel Mac
UNAME_M := $(shell uname -m)
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    # macOS
    ifeq ($(UNAME_M),arm64)
        # Apple Silicon Mac
        LIB_PATH = /opt/homebrew/lib
        INCLUDE_PATH = /opt/homebrew/include
    else
        # Intel Mac
        LIB_PATH = /usr/local/lib
        INCLUDE_PATH = /usr/local/include
    endif
    # macOS-specific libraries and frameworks
    LIBS = -L$(LIB_PATH) -lglfw3 -lGLEW -lassimp
    FRAMEWORKS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
else
    # Linux/Ubuntu
    LIB_PATH = /usr/local/lib
    INCLUDE_PATH = /usr/local/include
    # Linux-specific libraries (no frameworks)
    LIBS = -L$(LIB_PATH) -lglfw -lGLEW -lassimp -lGL
    FRAMEWORKS =
endif

# Include directories
INCLUDES = -I$(INCLUDE_PATH) -Iinclude

# VPATH for source files
VPATH = src

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LIBS) $(FRAMEWORKS)

# Compile source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)

# Install dependencies
install-deps:
	@echo "Installing dependencies for $(UNAME_S)..."
	@if [ "$(UNAME_S)" = "Darwin" ]; then \
		brew install glfw glew assimp glm; \
	else \
		sudo apt-get update && sudo apt-get install -y libglfw3-dev libglew-dev libassimp-dev libglm-dev libgl1-mesa-dev; \
	fi

# Run the application
execute: $(TARGET)
	./$(TARGET)

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

# Check dependencies
check-deps:
	@echo "Checking for required dependencies on $(UNAME_S)..."
	@if [ "$(UNAME_S)" = "Darwin" ]; then \
		brew list glfw > /dev/null 2>&1 || echo "❌ GLFW not found. Run 'make install-deps'"; \
		brew list glew > /dev/null 2>&1 || echo "❌ GLEW not found. Run 'make install-deps'"; \
		brew list assimp > /dev/null 2>&1 || echo "❌ Assimp not found. Run 'make install-deps'"; \
		brew list glm > /dev/null 2>&1 || echo "❌ GLM not found. Run 'make install-deps'"; \
	else \
		pkg-config --exists glfw3 || echo "❌ GLFW not found. Run 'make install-deps'"; \
		pkg-config --exists glew || echo "❌ GLEW not found. Run 'make install-deps'"; \
		pkg-config --exists assimp || echo "❌ Assimp not found. Run 'make install-deps'"; \
		pkg-config --exists glm || echo "❌ GLM not found. Run 'make install-deps'"; \
	fi
	@test -f include/stb_image.h || echo "❌ stb_image.h not found in include/ directory"
	@echo "✅ Dependency check complete"

# Help target
help:
	@echo "Available targets:"
	@echo "  all          - Build the application (default)"
	@echo "  clean        - Remove build artifacts"
	@echo "  install-deps - Install required dependencies via Homebrew"
	@echo "  execute      - Build and run the application"
	@echo "  debug        - Build with debug symbols"
	@echo "  check-deps   - Check if all dependencies are installed"
	@echo "  help         - Show this help message"

# Phony targets
.PHONY: all clean install-deps execute debug check-deps help
