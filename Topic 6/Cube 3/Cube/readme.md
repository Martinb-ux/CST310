# Install dependency (if not already installed)
sudo apt-get install freeglut3-dev

# Compile
g++ -std=c++17 -O2 -o specular_cubes specular_cubes.cpp -lGL -lGLU -lglut

# Run
./specular_cubes
