# Makefile for Quadrilateral Mesh Shading Demo
# Usage: make

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2 -DGL_SILENCE_DEPRECATION
TARGET = quadrilateral_mesh_demo
SOURCES = main.cpp quadrilateral_mesh.cpp

# OpenGL and GLUT libraries (macOS)
LDFLAGS = -framework OpenGL -framework GLUT

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS)

# Clean build artifacts
clean:
	rm -f $(TARGET)

# Install (copy to system path - optional)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Uninstall (remove from system path - optional)
uninstall:
	rm -f /usr/local/bin/$(TARGET)

# Run the demo
run: $(TARGET)
	./$(TARGET)

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

.PHONY: all clean install uninstall run debug
