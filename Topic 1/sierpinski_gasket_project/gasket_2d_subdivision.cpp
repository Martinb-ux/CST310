/*
 * Sierpinski Gasket - 2D Subdivision Method
 * 
 * This implementation uses recursive subdivision of triangles
 * to generate the Sierpinski Gasket. Each triangle is divided
 * into four smaller triangles, and the center one is removed.
 * 
 * Controls:
 * - ESC: Exit the program
 * - +/-: Increase/decrease subdivision depth
 * - R: Reset view
 * - SPACE: Toggle fill/wireframe mode
 */

#include <GLUT/glut.h>
#include <iostream>
#include <cmath>

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

// Subdivision depth (number of recursive iterations)
int subdivisionDepth = 5;
const int MAX_DEPTH = 8;
const int MIN_DEPTH = 0;

// Fill or wireframe mode
bool fillMode = true;

// Triangle vertices
typedef float point2[2];

// Initial triangle vertices
point2 vertices[3] = {
    {-0.9f, -0.9f},  // Bottom-left
    {0.9f, -0.9f},   // Bottom-right
    {0.0f, 0.9f}     // Top
};

/*
 * Initialize OpenGL settings
 */
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);  // Black background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    
    std::cout << "=== Sierpinski Gasket - 2D Subdivision Method ===" << std::endl;
    std::cout << "Current subdivision depth: " << subdivisionDepth << std::endl;
    std::cout << "Controls: +/- to adjust depth (0-" << MAX_DEPTH << "), " 
              << "SPACE to toggle fill, ESC to exit" << std::endl;
}

/*
 * Draw a single triangle
 */
void drawTriangle(point2 a, point2 b, point2 c) {
    glBegin(fillMode ? GL_TRIANGLES : GL_LINE_LOOP);
    glVertex2fv(a);
    glVertex2fv(b);
    glVertex2fv(c);
    glEnd();
}

/*
 * Recursive subdivision algorithm
 * 
 * This function divides a triangle into four smaller triangles
 * by connecting the midpoints of each side. It then recursively
 * subdivides three of the four triangles (excluding the center one).
 * 
 * Parameters:
 *   a, b, c: The three vertices of the current triangle
 *   depth: Current recursion depth
 * 
 * Algorithm:
 * 1. If depth reaches 0, draw the triangle
 * 2. Otherwise, calculate midpoints of all three sides
 * 3. Recursively subdivide the three corner triangles
 * 4. Skip the center triangle (this creates the gasket pattern)
 */
void subdivideTriangle(point2 a, point2 b, point2 c, int depth) {
    if (depth == 0) {
        // Base case: draw the triangle
        drawTriangle(a, b, c);
    } else {
        // Calculate midpoints
        point2 ab, bc, ca;
        
        // Midpoint of a and b
        ab[0] = (a[0] + b[0]) / 2.0f;
        ab[1] = (a[1] + b[1]) / 2.0f;
        
        // Midpoint of b and c
        bc[0] = (b[0] + c[0]) / 2.0f;
        bc[1] = (b[1] + c[1]) / 2.0f;
        
        // Midpoint of c and a
        ca[0] = (c[0] + a[0]) / 2.0f;
        ca[1] = (c[1] + a[1]) / 2.0f;
        
        // Recursively subdivide three outer triangles
        // Note: We don't subdivide the center triangle (ab, bc, ca)
        // This creates the Sierpinski pattern
        subdivideTriangle(a, ab, ca, depth - 1);   // Bottom-left
        subdivideTriangle(ab, b, bc, depth - 1);   // Bottom-right
        subdivideTriangle(ca, bc, c, depth - 1);   // Top
    }
}

/*
 * Display callback - renders the Sierpinski Gasket
 */
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Use a gradient color based on depth for visual interest
    float colorIntensity = 0.2f + (subdivisionDepth / (float)MAX_DEPTH) * 0.8f;
    glColor3f(colorIntensity, 0.5f, 1.0f - colorIntensity);
    
    // Start the recursive subdivision
    subdivideTriangle(vertices[0], vertices[1], vertices[2], subdivisionDepth);
    
    glFlush();
}

/*
 * Keyboard callback for interactive controls
 */
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27:  // ESC key
            exit(0);
            break;
        case '+':
        case '=':
            if (subdivisionDepth < MAX_DEPTH) {
                subdivisionDepth++;
                std::cout << "Subdivision depth increased to: " << subdivisionDepth << std::endl;
                glutPostRedisplay();
            } else {
                std::cout << "Maximum depth reached (" << MAX_DEPTH << ")" << std::endl;
            }
            break;
        case '-':
        case '_':
            if (subdivisionDepth > MIN_DEPTH) {
                subdivisionDepth--;
                std::cout << "Subdivision depth decreased to: " << subdivisionDepth << std::endl;
                glutPostRedisplay();
            } else {
                std::cout << "Minimum depth reached (" << MIN_DEPTH << ")" << std::endl;
            }
            break;
        case ' ':  // Space bar
            fillMode = !fillMode;
            std::cout << "Mode: " << (fillMode ? "Filled" : "Wireframe") << std::endl;
            glutPostRedisplay();
            break;
        case 'r':
        case 'R':
            subdivisionDepth = 5;
            fillMode = true;
            std::cout << "Reset to default settings" << std::endl;
            glutPostRedisplay();
            break;
    }
}

/*
 * Main function
 */
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Sierpinski Gasket - 2D Subdivision Method");
    
    init();
    
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    
    glutMainLoop();
    return 0;
}
