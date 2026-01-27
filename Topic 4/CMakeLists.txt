cmake_minimum_required(VERSION 3.10)
project(StairwellScene)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenGL REQUIRED)

# Try to find GLFW3
find_package(glfw3 QUIET)
if(NOT glfw3_FOUND)
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
endif()

# Try to find GLEW
find_package(GLEW QUIET)
if(NOT GLEW_FOUND)
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLEW REQUIRED glew)
endif()

# GLM is header-only, just need to find the headers
find_package(glm QUIET)

# Add executable
add_executable(stairwell_scene main.cpp)

# Include directories
target_include_directories(stairwell_scene PRIVATE 
    ${OPENGL_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
)

# Link libraries
if(glfw3_FOUND)
    target_link_libraries(stairwell_scene 
        OpenGL::GL
        glfw
        GLEW::GLEW
    )
else()
    target_link_libraries(stairwell_scene 
        ${OPENGL_LIBRARIES}
        ${GLFW_LIBRARIES}
        ${GLEW_LIBRARIES}
    )
endif()

# Copy shader files to build directory
configure_file(vertex_shader.glsl ${CMAKE_BINARY_DIR}/vertex_shader.glsl COPYONLY)
configure_file(fragment_shader.glsl ${CMAKE_BINARY_DIR}/fragment_shader.glsl COPYONLY)

# Platform-specific settings
if(APPLE)
    target_link_libraries(stairwell_scene "-framework Cocoa -framework IOKit")
endif()

# Set output directory
set_target_properties(stairwell_scene PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Print build information
message(STATUS "OpenGL found: ${OPENGL_FOUND}")
message(STATUS "GLFW found: ${glfw3_FOUND}")
message(STATUS "GLEW found: ${GLEW_FOUND}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
