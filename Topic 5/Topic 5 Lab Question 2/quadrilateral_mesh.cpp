#include "quadrilateral_mesh.h"
#include <iostream>

QuadrilateralMesh::QuadrilateralMesh() {
    vertices.clear();
    quads.clear();
}

void QuadrilateralMesh::addVertex(const Vertex& vertex) {
    vertices.push_back(vertex);
}

void QuadrilateralMesh::addQuad(const Quad& quad) {
    quads.push_back(quad);
}

void QuadrilateralMesh::calculateTriangleNormal(const Vertex& v1, const Vertex& v2, const Vertex& v3, 
                                               float& nx, float& ny, float& nz) const {
    // Calculate two edge vectors
    float edge1x = v2.x - v1.x;
    float edge1y = v2.y - v1.y;
    float edge1z = v2.z - v1.z;
    
    float edge2x = v3.x - v1.x;
    float edge2y = v3.y - v1.y;
    float edge2z = v3.z - v1.z;
    
    // Cross product to get normal
    nx = edge1y * edge2z - edge1z * edge2y;
    ny = edge1z * edge2x - edge1x * edge2z;
    nz = edge1x * edge2y - edge1y * edge2x;
    
    // Normalize
    float length = sqrt(nx * nx + ny * ny + nz * nz);
    if (length > 0.0001f) {
        nx /= length;
        ny /= length;
        nz /= length;
    }
}

void QuadrilateralMesh::createGridMesh(int width, int height, float spacing) {
    clear();
    
    // Create vertices
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            Vertex v;
            v.x = i * spacing - (width - 1) * spacing * 0.5f;
            v.y = 0.0f;
            v.z = j * spacing - (height - 1) * spacing * 0.5f;
            v.u = (float)i / (width - 1);
            v.v = (float)j / (height - 1);
            vertices.push_back(v);
        }
    }
    
    // Create quads
    for (int j = 0; j < height - 1; ++j) {
        for (int i = 0; i < width - 1; ++i) {
            int idx = j * width + i;
            Quad quad(idx, idx + 1, idx + width + 1, idx + width);
            quads.push_back(quad);
        }
    }
}

void QuadrilateralMesh::createHeightField(int width, int height, float (*heightFunc)(float, float), 
                                         float xMin, float xMax, float zMin, float zMax) {
    clear();
    
    // Create vertices with height function
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            float x = xMin + (xMax - xMin) * i / (width - 1);
            float z = zMin + (zMax - zMin) * j / (height - 1);
            float y = heightFunc(x, z);
            
            Vertex v;
            v.x = x;
            v.y = y;
            v.z = z;
            v.u = (float)i / (width - 1);
            v.v = (float)j / (height - 1);
            vertices.push_back(v);
        }
    }
    
    // Create quads
    for (int j = 0; j < height - 1; ++j) {
        for (int i = 0; i < width - 1; ++i) {
            int idx = j * width + i;
            Quad quad(idx, idx + 1, idx + width + 1, idx + width);
            quads.push_back(quad);
        }
    }
}

void QuadrilateralMesh::calculateFaceNormals() {
    for (auto& vertex : vertices) {
        vertex.nx = 0.0f;
        vertex.ny = 1.0f;
        vertex.nz = 0.0f;
    }
}

void QuadrilateralMesh::calculateNormals() {
    // Reset all normals
    for (auto& vertex : vertices) {
        vertex.nx = 0.0f;
        vertex.ny = 0.0f;
        vertex.nz = 0.0f;
    }
    
    // Calculate face normals and accumulate vertex normals
    for (const auto& quad : quads) {
        const Vertex& v0 = vertices[quad.v0];
        const Vertex& v1 = vertices[quad.v1];
        const Vertex& v2 = vertices[quad.v2];
        const Vertex& v3 = vertices[quad.v3];
        
        // Calculate normal for first triangle (v0, v1, v2)
        float nx1, ny1, nz1;
        calculateTriangleNormal(v0, v1, v2, nx1, ny1, nz1);
        
        // Calculate normal for second triangle (v0, v2, v3)
        float nx2, ny2, nz2;
        calculateTriangleNormal(v0, v2, v3, nx2, ny2, nz2);
        
        // Average the two normals for the quad
        float nx = (nx1 + nx2) * 0.5f;
        float ny = (ny1 + ny2) * 0.5f;
        float nz = (nz1 + nz2) * 0.5f;
        
        // Accumulate normals for each vertex
        vertices[quad.v0].nx += nx;
        vertices[quad.v0].ny += ny;
        vertices[quad.v0].nz += nz;
        
        vertices[quad.v1].nx += nx;
        vertices[quad.v1].ny += ny;
        vertices[quad.v1].nz += nz;
        
        vertices[quad.v2].nx += nx;
        vertices[quad.v2].ny += ny;
        vertices[quad.v2].nz += nz;
        
        vertices[quad.v3].nx += nx;
        vertices[quad.v3].ny += ny;
        vertices[quad.v3].nz += nz;
    }
    
    // Normalize all vertex normals
    for (auto& vertex : vertices) {
        float length = sqrt(vertex.nx * vertex.nx + vertex.ny * vertex.ny + vertex.nz * vertex.nz);
        if (length > 0.0001f) {
            vertex.nx /= length;
            vertex.ny /= length;
            vertex.nz /= length;
        } else {
            vertex.nx = 0.0f;
            vertex.ny = 1.0f;
            vertex.nz = 0.0f;
        }
    }
}

void QuadrilateralMesh::smoothNormals() {
    calculateNormals();
}

void QuadrilateralMesh::renderFlatShading() const {
    glShadeModel(GL_FLAT);
    
    for (const auto& quad : quads) {
        const Vertex& v0 = vertices[quad.v0];
        const Vertex& v1 = vertices[quad.v1];
        const Vertex& v2 = vertices[quad.v2];
        const Vertex& v3 = vertices[quad.v3];
        
        // Calculate face normal for flat shading
        float nx, ny, nz;
        calculateTriangleNormal(v0, v1, v2, nx, ny, nz);
        
        glBegin(GL_QUADS);
        glNormal3f(nx, ny, nz);
        glColor3f(v0.r, v0.g, v0.b);
        glVertex3f(v0.x, v0.y, v0.z);
        
        glColor3f(v1.r, v1.g, v1.b);
        glVertex3f(v1.x, v1.y, v1.z);
        
        glColor3f(v2.r, v2.g, v2.b);
        glVertex3f(v2.x, v2.y, v2.z);
        
        glColor3f(v3.r, v3.g, v3.b);
        glVertex3f(v3.x, v3.y, v3.z);
        glEnd();
    }
}

void QuadrilateralMesh::renderSmoothShading() const {
    glShadeModel(GL_SMOOTH);
    
    for (const auto& quad : quads) {
        const Vertex& v0 = vertices[quad.v0];
        const Vertex& v1 = vertices[quad.v1];
        const Vertex& v2 = vertices[quad.v2];
        const Vertex& v3 = vertices[quad.v3];
        
        glBegin(GL_QUADS);
        glNormal3f(v0.nx, v0.ny, v0.nz);
        glColor3f(v0.r, v0.g, v0.b);
        glVertex3f(v0.x, v0.y, v0.z);
        
        glNormal3f(v1.nx, v1.ny, v1.nz);
        glColor3f(v1.r, v1.g, v1.b);
        glVertex3f(v1.x, v1.y, v1.z);
        
        glNormal3f(v2.nx, v2.ny, v2.nz);
        glColor3f(v2.r, v2.g, v2.b);
        glVertex3f(v2.x, v2.y, v2.z);
        
        glNormal3f(v3.nx, v3.ny, v3.nz);
        glColor3f(v3.r, v3.g, v3.b);
        glVertex3f(v3.x, v3.y, v3.z);
        glEnd();
    }
}

void QuadrilateralMesh::renderGouraudShading() const {
    // Gouraud shading is essentially smooth shading with per-vertex lighting
    renderSmoothShading();
}

void QuadrilateralMesh::renderPhongShading() const {
    // Note: True Phong shading requires fragment shaders
    // This is an approximation using smooth shading with enhanced normals
    renderSmoothShading();
}

void QuadrilateralMesh::clear() {
    vertices.clear();
    quads.clear();
}

void QuadrilateralMesh::setColorsFromHeight(float minHeight, float maxHeight) {
    for (auto& vertex : vertices) {
        float t = (vertex.y - minHeight) / (maxHeight - minHeight);
        t = std::max(0.0f, std::min(1.0f, t));
        
        // Create a gradient from blue (low) to red (high)
        vertex.r = t;
        vertex.g = 0.2f + 0.3f * (1.0f - fabs(t - 0.5f) * 2.0f);
        vertex.b = 1.0f - t;
    }
}

void QuadrilateralMesh::setColorsFromSlope() {
    for (size_t i = 0; i < vertices.size(); ++i) {
        const Vertex& v = vertices[i];
        float slope = sqrt(v.nx * v.nx + v.nz * v.nz); // Horizontal component of normal
        
        // Color based on slope: green for flat, red for steep
        vertices[i].r = slope;
        vertices[i].g = 1.0f - slope;
        vertices[i].b = 0.2f;
    }
}
