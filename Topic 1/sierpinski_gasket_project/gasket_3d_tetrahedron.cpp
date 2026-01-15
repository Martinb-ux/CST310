/*
 * Sierpinski Gasket - 3D Tetrahedron Method
 * 
 * This implementation extends the Sierpinski Gasket concept to 3D
 * by recursively subdividing a tetrahedron (3D pyramid with 4 triangular faces).
 * Each tetrahedron is divided into 4 smaller tetrahedra at each iteration.
 * 
 * Controls:
 * - ESC: Exit the program
 * - +/-: Increase/decrease subdivision depth
 * - Arrow Keys: Rotate the tetrahedron
 * - R: Reset rotation
 * - SPACE: Toggle rotation animation
 * - W: Toggle wireframe mode
 */

#include <GLUT/glut.h>
#include <iostream>
#include <cmath>

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

// Subdivision depth
int subdivisionDepth = 4;
const int MAX_DEPTH = 6;
const int MIN_DEPTH = 0;

// Rotation angles
float rotationX = 30.0f;
float rotationY = 45.0f;
float rotationZ = 0.0f;

// Animation control
bool animating = false;
float animationSpeed = 0.5f;

// Wireframe mode
bool wireframeMode = false;

// 3D point structure
typedef float point3[3];

// Tetrahedron vertices (regular tetrahedron centered at origin)
point3 vertices[4] = {
    {0.0f, 1.0f, 0.0f},           // Top vertex
    {0.0f, -0.5f, 0.866f},        // Front vertex
    {-0.866f, -0.5f, -0.433f},    // Back-left vertex
    {0.866f, -0.5f, -0.433f}      // Back-right vertex
};

/*
 * Initialize OpenGL settings
 */
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);  // Black background
    
    // Enable depth testing for 3D
    glEnable(GL_DEPTH_TEST);
    
    // Set up projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 0.1, 100.0);
    
    // Set up modelview
    glMatrixMode(GL_MODELVIEW);
    
    // Enable smooth shading
    glShadeModel(GL_SMOOTH);
    
    std::cout << "=== Sierpinski Gasket - 3D Tetrahedron Method ===" << std::endl;
    std::cout << "Current subdivision depth: " << subdivisionDepth << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  +/- : Adjust depth (0-" << MAX_DEPTH << ")" << std::endl;
    std::cout << "  Arrow keys: Rotate" << std::endl;
    std::cout << "  SPACE: Toggle animation" << std::endl;
    std::cout << "  W: Toggle wireframe" << std::endl;
    std::cout << "  R: Reset rotation" << std::endl;
    std::cout << "  ESC: Exit" << std::endl;
}

/*
 * Calculate normal vector for a triangle (for lighting)
 */
void calculateNormal(point3 a, point3 b, point3 c, float normal[3]) {
    float v1[3], v2[3];
    
    // Calculate two edge vectors
    v1[0] = b[0] - a[0];
    v1[1] = b[1] - a[1];
    v1[2] = b[2] - a[2];
    
    v2[0] = c[0] - a[0];
    v2[1] = c[1] - a[1];
    v2[2] = c[2] - a[2];
    
    // Cross product
    normal[0] = v1[1] * v2[2] - v1[2] * v2[1];
    normal[1] = v1[2] * v2[0] - v1[0] * v2[2];
    normal[2] = v1[0] * v2[1] - v1[1] * v2[0];
    
    // Normalize
    float length = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
    if (length > 0.0f) {
        normal[0] /= length;
        normal[1] /= length;
        normal[2] /= length;
    }
}

/*
 * Draw a single triangle with color
 */
void drawTriangle(point3 a, point3 b, point3 c, float r, float g, float blue) {
    if (wireframeMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    
    // Calculate and set normal for lighting
    float normal[3];
    calculateNormal(a, b, c, normal);
    glNormal3fv(normal);
    
    glColor3f(r, g, blue);
    glBegin(GL_TRIANGLES);
    glVertex3fv(a);
    glVertex3fv(b);
    glVertex3fv(c);
    glEnd();
}

/*
 * Recursive tetrahedron subdivision
 * 
 * Algorithm:
 * 1. If depth is 0, draw the four faces of the tetrahedron
 * 2. Otherwise, calculate midpoints of all six edges
 * 3. Recursively subdivide four corner tetrahedra
 * 
 * This creates a 3D fractal structure where each tetrahedron
 * is replaced by four smaller tetrahedra at its corners.
 */
void subdivideTetrahedron(point3 a, point3 b, point3 c, point3 d, int depth) {
    if (depth == 0) {
        // Base case: draw the four triangular faces with different colors
        drawTriangle(a, b, c, 1.0f, 0.0f, 0.0f);  // Red
        drawTriangle(a, c, d, 0.0f, 1.0f, 0.0f);  // Green
        drawTriangle(a, d, b, 0.0f, 0.0f, 1.0f);  // Blue
        drawTriangle(b, d, c, 1.0f, 1.0f, 0.0f);  // Yellow
    } else {
        // Calculate midpoints of all six edges
        point3 ab, ac, ad, bc, bd, cd;
        
        for (int i = 0; i < 3; i++) {
            ab[i] = (a[i] + b[i]) / 2.0f;
            ac[i] = (a[i] + c[i]) / 2.0f;
            ad[i] = (a[i] + d[i]) / 2.0f;
            bc[i] = (b[i] + c[i]) / 2.0f;
            bd[i] = (b[i] + d[i]) / 2.0f;
            cd[i] = (c[i] + d[i]) / 2.0f;
        }
        
        // Recursively subdivide four corner tetrahedra
        subdivideTetrahedron(a, ab, ac, ad, depth - 1);   // Top
        subdivideTetrahedron(ab, b, bc, bd, depth - 1);   // Front
        subdivideTetrahedron(ac, bc, c, cd, depth - 1);   // Left
        subdivideTetrahedron(ad, bd, cd, d, depth - 1);   // Right
    }
}

/*
 * Display callback
 */
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    
    // Position camera
    gluLookAt(0.0, 0.0, 3.0,   // Eye position
              0.0, 0.0, 0.0,   // Look at point
              0.0, 1.0, 0.0);  // Up vector
    
    // Apply rotations
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotationZ, 0.0f, 0.0f, 1.0f);
    
    // Scale to fit in view
    glScalef(0.8f, 0.8f, 0.8f);
    
    // Draw the 3D Sierpinski gasket
    subdivideTetrahedron(vertices[0], vertices[1], vertices[2], vertices[3], subdivisionDepth);
    
    glutSwapBuffers();
}

/*
 * Idle callback for animation
 */
void idle() {
    if (animating) {
        rotationY += animationSpeed;
        if (rotationY > 360.0f) rotationY -= 360.0f;
        glutPostRedisplay();
    }
}

/*
 * Keyboard callback
 */
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27:  // ESC
            exit(0);
            break;
        case '+':
        case '=':
            if (subdivisionDepth < MAX_DEPTH) {
                subdivisionDepth++;
                std::cout << "Subdivision depth: " << subdivisionDepth << std::endl;
                glutPostRedisplay();
            }
            break;
        case '-':
        case '_':
            if (subdivisionDepth > MIN_DEPTH) {
                subdivisionDepth--;
                std::cout << "Subdivision depth: " << subdivisionDepth << std::endl;
                glutPostRedisplay();
            }
            break;
        case ' ':  // Space
            animating = !animating;
            std::cout << "Animation: " << (animating ? "ON" : "OFF") << std::endl;
            break;
        case 'w':
        case 'W':
            wireframeMode = !wireframeMode;
            std::cout << "Mode: " << (wireframeMode ? "Wireframe" : "Filled") << std::endl;
            glutPostRedisplay();
            break;
        case 'r':
        case 'R':
            rotationX = 30.0f;
            rotationY = 45.0f;
            rotationZ = 0.0f;
            animating = false;
            std::cout << "Reset rotation" << std::endl;
            glutPostRedisplay();
            break;
    }
}

/*
 * Special key callback for arrow keys
 */
void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            rotationX += 5.0f;
            break;
        case GLUT_KEY_DOWN:
            rotationX -= 5.0f;
            break;
        case GLUT_KEY_LEFT:
            rotationY -= 5.0f;
            break;
        case GLUT_KEY_RIGHT:
            rotationY += 5.0f;
            break;
    }
    glutPostRedisplay();
}

/*
 * Window reshape callback
 */
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w / (float)h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

/*
 * Main function
 */
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Sierpinski Gasket - 3D Tetrahedron Method");
    
    init();
    
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    
    glutMainLoop();
    return 0;
}
