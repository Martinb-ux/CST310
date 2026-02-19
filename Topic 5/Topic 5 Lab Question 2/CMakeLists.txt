cmake_minimum_required(VERSION 3.10)
project(QuadrilateralMeshShading)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenGL and GLUT
find_package(OpenGL REQUIRED)

# On macOS, GLUT is part of the system framework
if(APPLE)
    find_package(GLUT REQUIRED)
    set(GLUT_LIBRARIES "-framework GLUT")
else()
    find_package(GLUT REQUIRED)
endif()

# Create executable
add_executable(quadrilateral_mesh_demo
    main.cpp
    quadrilateral_mesh.cpp
)

# Link libraries
target_link_libraries(quadrilateral_mesh_demo
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
)

# Compiler-specific settings
if(MSVC)
    target_compile_definitions(quadrilateral_mesh_demo PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# Set output directory
set_target_properties(quadrilateral_mesh_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
