// Quadrilateral Mesh Demo with Shading Algorithms
// Demonstrates different shading techniques on quadrilateral meshes
// Controls: Arrow keys rotate, +/- zoom, 1-4 switch shading modes, Esc quit

#include <cmath>
#include <algorithm>
#include <iostream>
#include "quadrilateral_mesh.h"

static int gWinW = 800, gWinH = 600;
static float gYaw = -45.0f, gPitch = 30.0f;
static float gZoom = 10.0f;
static int gShadingMode = 1; // 1=Flat, 2=Smooth, 3=Gouraud, 4=Phong

static QuadrilateralMesh gMesh;

// Height function for interesting terrain
static float heightFunction(float x, float z) {
    return 2.0f * std::sin(x * 0.5f) * std::cos(z * 0.5f) + 
           0.5f * std::sin(x * 2.0f) * std::sin(z * 2.0f) +
           0.3f * std::cos(x * 3.0f + z * 2.0f);
}

// Lighting setup
static void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    
    // Main light
    GLfloat light0_pos[] = {5.0f, 10.0f, 5.0f, 1.0f};
    GLfloat light0_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat light0_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat light0_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    
    // Fill light
    GLfloat light1_pos[] = {-3.0f, 5.0f, -3.0f, 1.0f};
    GLfloat light1_diffuse[] = {0.3f, 0.3f, 0.4f, 1.0f};
    
    glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    
    // Material properties
    GLfloat material_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat material_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat material_specular[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat material_shininess[] = {32.0f};
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);
    
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

static void display() {
    glViewport(0, 0, gWinW, gWinH);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)gWinW / (double)gWinH, 0.1, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, -2.0f, -gZoom);
    glRotatef(gPitch, 1, 0, 0);
    glRotatef(gYaw, 0, 1, 0);
    
    // Setup lighting
    setupLighting();
    
    // Render mesh based on current shading mode
    switch (gShadingMode) {
        case 1:
            std::cout << "Rendering with Flat Shading" << std::endl;
            glDisable(GL_LIGHTING);
            gMesh.renderFlatShading();
            break;
        case 2:
            std::cout << "Rendering with Smooth Shading" << std::endl;
            glEnable(GL_LIGHTING);
            gMesh.renderSmoothShading();
            break;
        case 3:
            std::cout << "Rendering with Gouraud Shading" << std::endl;
            glEnable(GL_LIGHTING);
            gMesh.renderGouraudShading();
            break;
        case 4:
            std::cout << "Rendering with Phong Shading (approximation)" << std::endl;
            glEnable(GL_LIGHTING);
            gMesh.renderPhongShading();
            break;
    }
    
    // Draw coordinate axes
    glDisable(GL_LIGHTING);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    // X axis - red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(2.0f, 0.0f, 0.0f);
    // Y axis - green
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 2.0f, 0.0f);
    // Z axis - blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 2.0f);
    glEnd();
    
    glutSwapBuffers();
}

static void reshape(int w, int h) {
    gWinW = std::max(1, w);
    gWinH = std::max(1, h);
}

static void keyboard(unsigned char key, int, int) {
    switch (key) {
        case 27: 
            std::exit(0); 
            break;
        case '+': 
            gZoom = std::max(3.0f, gZoom - 0.5f); 
            break;
        case '-': 
            gZoom = std::min(30.0f, gZoom + 0.5f); 
            break;
        case '1':
            gShadingMode = 1;
            std::cout << "Switched to Flat Shading" << std::endl;
            break;
        case '2':
            gShadingMode = 2;
            std::cout << "Switched to Smooth Shading" << std::endl;
            break;
        case '3':
            gShadingMode = 3;
            std::cout << "Switched to Gouraud Shading" << std::endl;
            break;
        case '4':
            gShadingMode = 4;
            std::cout << "Switched to Phong Shading" << std::endl;
            break;
        case 'h':
            // Toggle between height-based and slope-based coloring
            static bool useHeightColoring = true;
            if (useHeightColoring) {
                float minHeight = -5.0f, maxHeight = 5.0f;
                gMesh.setColorsFromHeight(minHeight, maxHeight);
                std::cout << "Switched to height-based coloring" << std::endl;
            } else {
                gMesh.setColorsFromSlope();
                std::cout << "Switched to slope-based coloring" << std::endl;
            }
            useHeightColoring = !useHeightColoring;
            break;
    }
    glutPostRedisplay();
}

static void special(int key, int, int) {
    const float step = 5.0f;
    switch (key) {
        case GLUT_KEY_LEFT:  gYaw   -= step; break;
        case GLUT_KEY_RIGHT: gYaw   += step; break;
        case GLUT_KEY_UP:    gPitch -= step; break;
        case GLUT_KEY_DOWN:  gPitch += step; break;
    }
    glutPostRedisplay();
}

static void initMesh() {
    // Create a height field mesh
    const int meshWidth = 40;
    const int meshHeight = 40;
    
    gMesh.createHeightField(meshWidth, meshHeight, heightFunction, 
                           -8.0f, 8.0f, -8.0f, 8.0f);
    
    // Calculate normals for proper lighting
    gMesh.calculateNormals();
    
    // Set colors based on height
    float minHeight = -5.0f, maxHeight = 5.0f;
    gMesh.setColorsFromHeight(minHeight, maxHeight);
    
    std::cout << "Created quadrilateral mesh with " << gMesh.getVertexCount() 
              << " vertices and " << gMesh.getQuadCount() << " quads." << std::endl;
}

void printControls() {
    std::cout << "\n=== Quadrilateral Mesh Shading Demo ===" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  Arrow Keys: Rotate view" << std::endl;
    std::cout << "  +/-: Zoom in/out" << std::endl;
    std::cout << "  1: Flat shading" << std::endl;
    std::cout << "  2: Smooth shading" << std::endl;
    std::cout << "  3: Gouraud shading" << std::endl;
    std::cout << "  4: Phong shading (approximation)" << std::endl;
    std::cout << "  h: Toggle height/slope coloring" << std::endl;
    std::cout << "  Esc: Quit" << std::endl;
    std::cout << "=====================================\n" << std::endl;
}

int main(int argc, char** argv) {
    printControls();
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(gWinW, gWinH);
    glutCreateWindow("Quadrilateral Mesh - Shading Algorithms");
    
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    
    // Initialize the mesh
    initMesh();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    
    glutMainLoop();
    return 0;
}
