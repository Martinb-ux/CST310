#ifndef QUADRILATERAL_MESH_H
#define QUADRILATERAL_MESH_H

#include <vector>
#include <cmath>
#include <algorithm>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Vertex structure for quadrilateral mesh
struct Vertex {
    float x, y, z;           // Position
    float nx, ny, nz;         // Normal vector for lighting
    float u, v;               // Texture coordinates
    float r, g, b;            // Color
    
    Vertex() : x(0), y(0), z(0), nx(0), ny(1), nz(0), u(0), v(0), r(1), g(1), b(1) {}
    Vertex(float px, float py, float pz) 
        : x(px), y(py), z(pz), nx(0), ny(1), nz(0), u(0), v(0), r(1), g(1), b(1) {}
};

// Quadrilateral face structure
struct Quad {
    int v0, v1, v2, v3;      // Vertex indices
    
    Quad() : v0(0), v1(0), v2(0), v3(0) {}
    Quad(int a, int b, int c, int d) : v0(a), v1(b), v2(c), v3(d) {}
};

// Quadrilateral mesh data structure
class QuadrilateralMesh {
private:
    std::vector<Vertex> vertices;
    std::vector<Quad> quads;
    
    // Calculate normal for a triangle (helper for quad normal calculation)
    void calculateTriangleNormal(const Vertex& v1, const Vertex& v2, const Vertex& v3, 
                                float& nx, float& ny, float& nz) const;
    
public:
    // Constructor
    QuadrilateralMesh();
    
    // Mesh construction methods
    void addVertex(const Vertex& vertex);
    void addQuad(const Quad& quad);
    void createGridMesh(int width, int height, float spacing = 1.0f);
    void createHeightField(int width, int height, float (*heightFunc)(float, float), 
                          float xMin = -5.0f, float xMax = 5.0f, 
                          float zMin = -5.0f, float zMax = 5.0f);
    
    // Normal calculation
    void calculateNormals();
    void calculateFaceNormals();
    void smoothNormals();
    
    // Shading methods
    void renderFlatShading() const;
    void renderSmoothShading() const;
    void renderGouraudShading() const;
    void renderPhongShading() const;
    
    // Utility methods
    void clear();
    int getVertexCount() const { return vertices.size(); }
    int getQuadCount() const { return quads.size(); }
    const Vertex& getVertex(int index) const { return vertices[index]; }
    const Quad& getQuad(int index) const { return quads[index]; }
    
    // Color mapping based on height or other properties
    void setColorsFromHeight(float minHeight, float maxHeight);
    void setColorsFromSlope();
};

#endif
