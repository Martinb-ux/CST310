/*
 * Sierpinski Gasket - 2D Random Point Method
 * 
 * This implementation uses the chaos game algorithm to generate
 * the Sierpinski Gasket by randomly selecting vertices and 
 * plotting midpoints iteratively.
 * 
 * Controls:
 * - ESC: Exit the program
 * - R: Reset and regenerate with current parameters
 * - +/-: Increase/decrease number of iterations
 */

#include <GLUT/glut.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

// Number of points to generate (adjustable)
int numPoints = 50000;

// Triangle vertices for 2D gasket
float vertices[3][2] = {
    {-0.9f, -0.9f},  // Bottom-left
    {0.9f, -0.9f},   // Bottom-right
    {0.0f, 0.9f}     // Top
};

// Current point position
float currentPoint[2] = {0.0f, 0.0f};

// Color scheme
float pointColor[3] = {0.0f, 1.0f, 0.0f}; // Green

/*
 * Initialize OpenGL settings
 */
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);  // Black background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    
    // Initialize random seed
    srand(static_cast<unsigned>(time(0)));
    
    std::cout << "=== Sierpinski Gasket - 2D Random Point Method ===" << std::endl;
    std::cout << "Current points: " << numPoints << std::endl;
    std::cout << "Controls: +/- to adjust points, R to reset, ESC to exit" << std::endl;
}

/*
 * Calculate midpoint between two points
 */
void calculateMidpoint(float p1[2], float p2[2], float result[2]) {
    result[0] = (p1[0] + p2[0]) / 2.0f;
    result[1] = (p1[1] + p2[1]) / 2.0f;
}

/*
 * Display callback - renders the Sierpinski Gasket
 * 
 * Algorithm:
 * 1. Start with an arbitrary point inside the triangle
 * 2. Randomly select one of the three vertices
 * 3. Plot the midpoint between current point and selected vertex
 * 4. Make this midpoint the new current point
 * 5. Repeat steps 2-4 for the specified number of iterations
 */
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw the initial triangle vertices (for reference)
    glColor3f(1.0f, 0.0f, 0.0f);  // Red
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 3; i++) {
        glVertex2fv(vertices[i]);
    }
    glEnd();
    
    // Initialize starting point at the center
    currentPoint[0] = 0.0f;
    currentPoint[1] = 0.0f;
    
    // Generate the gasket points
    glColor3fv(pointColor);
    glPointSize(1.0f);
    glBegin(GL_POINTS);
    
    for (int i = 0; i < numPoints; i++) {
        // Randomly select one of the three vertices
        int randomVertex = rand() % 3;
        
        // Calculate midpoint between current point and selected vertex
        float midpoint[2];
        calculateMidpoint(currentPoint, vertices[randomVertex], midpoint);
        
        // Plot the midpoint
        glVertex2fv(midpoint);
        
        // Update current point
        currentPoint[0] = midpoint[0];
        currentPoint[1] = midpoint[1];
    }
    
    glEnd();
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
            numPoints += 5000;
            std::cout << "Points increased to: " << numPoints << std::endl;
            glutPostRedisplay();
            break;
        case '-':
        case '_':
            if (numPoints > 5000) {
                numPoints -= 5000;
                std::cout << "Points decreased to: " << numPoints << std::endl;
                glutPostRedisplay();
            }
            break;
        case 'r':
        case 'R':
            std::cout << "Regenerating gasket..." << std::endl;
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
    glutCreateWindow("Sierpinski Gasket - 2D Random Point Method");
    
    init();
    
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    
    glutMainLoop();
    return 0;
}
