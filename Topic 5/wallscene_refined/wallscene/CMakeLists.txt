# CMakeLists.txt for Project 4: Stairwell Scene
# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(StairwellScene VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

# Find GLM (header-only library)
find_package(glm REQUIRED)

# Add executable
add_executable(stairwell_scene src/main.cpp)

# Include directories
target_include_directories(stairwell_scene PRIVATE
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(stairwell_scene
    OpenGL::GL
    glfw
    GLEW::GLEW
    glm::glm
)

# macOS-specific settings
if(APPLE)
    target_link_libraries(stairwell_scene
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()

# Copy shader files to build directory and bin directory
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR}/bin)

# Set output directory
set_target_properties(stairwell_scene PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
