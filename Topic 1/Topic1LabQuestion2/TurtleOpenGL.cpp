// TurtleOpenGL.cpp
// Minimal Turtle Graphics-style system using OpenGL + GLUT.
// Demonstrates how a developer might implement a small turtle library
// with functions like:
//   - TurtleSetPosition(x, y)
//   - TurtleRotateLeft(angleDeg)
//   - TurtleMoveForward(distance)
//   - TurtlePenUp() / TurtlePenDown()
//   - TurtleSetColor(r, g, b)
//
// COMPILE EXAMPLES:
//
// macOS (Apple GLUT):
//   g++ TurtleOpenGL.cpp -o turtle -framework OpenGL -framework GLUT
//
// Linux (freeglut):
//   g++ TurtleOpenGL.cpp -o turtle -lGL -lGLU -lglut
//
// Windows (with freeglut installed and in your include/lib paths):
//   g++ TurtleOpenGL.cpp -o turtle -lfreeglut -lopengl32 -lglu32

#include <cmath>
#include <iostream>

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

// Mathematical constants
constexpr float PI = 3.14159265358979323846f;

// ------------------------ TURTLE STATE ------------------------

struct Turtle {
    float x;
    float y;
    float angleDeg;   // 0 degrees = facing right (+X)
    bool penDown;
    float r, g, b;    // current drawing color
};

Turtle gTurtle;

// ------------------------ TURTLE API --------------------------

// SetPosition(x, y): move turtle without drawing.
void TurtleSetPosition(float x, float y) {
    gTurtle.x = x;
    gTurtle.y = y;
}

// RotateLeft(angleDeg): rotate CCW by angleDeg.
void TurtleRotateLeft(float angleDeg) {
    gTurtle.angleDeg += angleDeg;
    // keep angle in [0,360) for easier debugging
    gTurtle.angleDeg = std::fmod(gTurtle.angleDeg, 360.0f);
    if (gTurtle.angleDeg < 0.0f) gTurtle.angleDeg += 360.0f;
}

// Optional: RotateRight(angleDeg) = negative turn.
void TurtleRotateRight(float angleDeg) {
    gTurtle.angleDeg -= angleDeg;
    // normalize
    gTurtle.angleDeg = std::fmod(gTurtle.angleDeg, 360.0f);
    if (gTurtle.angleDeg < 0.0f) gTurtle.angleDeg += 360.0f;
}

// PenUp(): turtle moves without drawing.
void TurtlePenUp() {
    gTurtle.penDown = false;
}

// PenDown(): turtle draws while moving.
void TurtlePenDown() {
    gTurtle.penDown = true;
}

// SetColor(r, g, b): set current drawing color (0–1 range).
void TurtleSetColor(float r, float g, float b) {
    gTurtle.r = r;
    gTurtle.g = g;
    gTurtle.b = b;
}

// MoveForward(distance): move in current direction, draw line if penDown.
void TurtleMoveForward(float distance) {
    float angleRad = gTurtle.angleDeg * PI / 180.0f;

    float x0 = gTurtle.x;
    float y0 = gTurtle.y;

    float x1 = x0 + distance * std::cos(angleRad);
    float y1 = y0 + distance * std::sin(angleRad);

    if (gTurtle.penDown) {
        glColor3f(gTurtle.r, gTurtle.g, gTurtle.b);
        glBegin(GL_LINES);
            glVertex2f(x0, y0);
            glVertex2f(x1, y1);
        glEnd();
    }

    gTurtle.x = x1;
    gTurtle.y = y1;
}

// MoveTo(x,y,draw): move to absolute coordinates; if draw==true draw a line
// from current position to (x,y). Units are in the current projection space.
void TurtleMoveTo(float x, float y, bool draw = true) {
    float x0 = gTurtle.x;
    float y0 = gTurtle.y;
    float x1 = x;
    float y1 = y;

    if (draw) {
        glColor3f(gTurtle.r, gTurtle.g, gTurtle.b);
        glBegin(GL_LINES);
            glVertex2f(x0, y0);
            glVertex2f(x1, y1);
        glEnd();
    }

    gTurtle.x = x1;
    gTurtle.y = y1;
}

// ------------------------ DEMO DRAWING ------------------------

// This function demonstrates calling the turtle API to draw
// a simple pattern (a square + a triangle).
void TurtleDemoDrawing() {
    // Reset turtle to a known state
    gTurtle.x = -0.5f;
    gTurtle.y = -0.5f;
    gTurtle.angleDeg = 0.0f;
    gTurtle.penDown = true;
    gTurtle.r = 0.0f;
    gTurtle.g = 1.0f;
    gTurtle.b = 0.0f;

    // Draw a square
    for (int i = 0; i < 4; ++i) {
        TurtleMoveForward(0.5f);
        TurtleRotateLeft(90.0f);
    }

    // Move to a new starting point without drawing
    TurtlePenUp();
    TurtleSetPosition(0.2f, -0.2f);
    TurtlePenDown();
    TurtleSetColor(1.0f, 0.0f, 0.0f);  // red

    // Draw a triangle
    for (int i = 0; i < 3; ++i) {
        TurtleMoveForward(0.4f);
        TurtleRotateLeft(120.0f);
    }

    // Another example: a small "ray" fan using RotateRight
    TurtlePenUp();
    TurtleSetPosition(0.0f, 0.0f);
    TurtleSetColor(0.0f, 0.7f, 1.0f);  // cyan-ish
    TurtlePenDown();

    for (int i = 0; i < 12; ++i) {
        TurtleMoveForward(0.6f);
        TurtlePenUp();
        TurtleMoveForward(-0.6f); // move back without drawing
        TurtlePenDown();
        TurtleRotateRight(30.0f);
    }
}

// ------------------------ OPENGL SETUP ------------------------

void DisplayCallback() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Reset modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // All turtle drawing happens here:
    TurtleDemoDrawing();

    glutSwapBuffers();
}

void ReshapeCallback(int width, int height) {
    glViewport(0, 0, width, height);

    // Simple orthographic projection from -1..1 in both axes
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Keep aspect ratio correct
    float aspect = (height == 0) ? 1.0f : (float)width / (float)height;
    if (aspect >= 1.0f) {
        // Wider than tall
        glOrtho(-aspect, aspect, -1.0, 1.0, -1.0, 1.0);
    } else {
        // Taller than wide
        glOrtho(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect, -1.0, 1.0);
    }
}

// Basic initialization
void InitGL() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // dark background
}

// ----------------------------- MAIN -----------------------------

int main(int argc, char** argv) {
    // Initialize turtle defaults
    gTurtle.x = 0.0f;
    gTurtle.y = 0.0f;
    gTurtle.angleDeg = 0.0f;
    gTurtle.penDown = true;
    gTurtle.r = 1.0f;
    gTurtle.g = 1.0f;
    gTurtle.b = 1.0f;

    // GLUT setup
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Turtle Graphics Example");

    InitGL();

    glutDisplayFunc(DisplayCallback);
    glutReshapeFunc(ReshapeCallback);

    glutMainLoop();
    return 0;
}
