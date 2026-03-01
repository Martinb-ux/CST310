// =============================================================================
// Specular Lighting / Shininess Demo — OpenGL / GLUT
// =============================================================================
//
// Author:  Martin
// Course:  Computer Graphics
// Date:    February 2026
//
// Description:
//   This program demonstrates how the specular shininess exponent (n) in the
//   Phong reflection model affects the visual appearance of a lit surface.
//   Eight cubes are rendered in a 4x2 grid with shininess values:
//       2, 4, 8, 16, 32, 64, 128, 256
//
//   After displaying the grid, the program queries the user (via terminal)
//   to enter a custom shininess value and displays a single cube with that
//   exponent.
//
// Theoretical Background:
//   The Phong reflection model computes fragment color as:
//
//       I = I_a * k_a  +  I_d * k_d * max(0, N·L)  +  I_s * k_s * max(0, R·V)^n
//
//   where:
//     I_a, I_d, I_s  = ambient, diffuse, specular light intensities
//     k_a, k_d, k_s  = material ambient, diffuse, specular coefficients
//     N              = surface normal (unit vector)
//     L              = light direction (unit vector, toward light)
//     V              = view direction (unit vector, toward camera)
//     R              = reflection of -L about N:  R = 2(N·L)N - L
//     n              = shininess exponent (the variable under study)
//
//   A small n (e.g. 2) produces a broad, soft specular highlight.
//   A large n (e.g. 256) produces a tight, bright pinpoint highlight.
//
//   Each cube face is subdivided into an 80x80 mesh of small quads.  This is
//   necessary because OpenGL's fixed-function pipeline computes lighting
//   per-vertex (Gouraud shading).  A single flat quad has only 4 vertices,
//   so the specular intensity barely varies across the face.  By subdividing,
//   we get 6,400+ vertices per face, allowing the specular highlight to
//   appear as a smooth gradient.
//
// Shininess Values Explained:
//   n =   2  — Very broad, diffuse highlight; almost uniform brightness
//   n =   4  — Slightly more focused; subtle gradient visible
//   n =   8  — Moderate spread; gentle bright area on front face
//   n =  16  — Noticeable highlight region forming
//   n =  32  — Clear circular highlight visible on front face
//   n =  64  — Tight highlight; surrounding area noticeably darker
//   n = 128  — Small bright spot; sharp falloff
//   n = 256  — Pinpoint highlight; very tight, intense specular dot
//
// Algorithm / Flow:
//   1. Initialize OpenGL window and lighting state
//   2. For each of the 8 grid cells:
//      a. Set viewport to cell region (upper portion for cube, lower for label)
//      b. Configure camera (perspective projection, gluLookAt)
//      c. Set light position and material properties (with shininess n)
//      d. Draw subdivided cube mesh (6 faces × 80×80 quads)
//      e. Render shininess label text below cube
//   3. Swap buffers to display the grid
//   4. Enter terminal query loop:
//      a. Prompt user for shininess value
//      b. If valid: render single cube full-window with that shininess
//      c. If 0: return to grid view
//      d. If 'q': exit
//
// Mesh / Illumination Algorithm:
//   For each cube face:
//     - Define origin corner, two edge vectors (u, v), and outward normal
//     - Subdivide into SUBDIV × SUBDIV quads using GL_QUAD_STRIP
//     - All vertices on a face share the same normal → lighting varies
//       only due to vertex position (affecting L, V, R vectors)
//   Illumination:
//     - GL_LIGHT0: positional light in front of cube
//     - GL_LIGHT_MODEL_LOCAL_VIEWER: true (accurate specular)
//     - Material: copper/orange diffuse, warm-tinted specular
//     - GL_SMOOTH shading: Gouraud interpolation across subdivided mesh
//
// =============================================================================
//
// Compile & Run:
//
//   --- Linux (Ubuntu/Debian) ---
//   Install dependencies:
//     sudo apt-get install freeglut3-dev
//   Compile:
//     g++ -std=c++17 -O2 -o specular_cubes specular_cubes.cpp -lGL -lGLU -lglut
//   Run:
//     ./specular_cubes
//
//   --- macOS ---
//   Compile:
//     g++ -std=c++17 -O2 -framework OpenGL -framework GLUT \
//         -Wno-deprecated-declarations -o specular_cubes specular_cubes.cpp
//   Run:
//     ./specular_cubes
//
// Controls:
//   ESC / q  — quit the program
//   g        — return to grid view (from single-cube query mode)
//   Terminal  — enter shininess value, 0 = grid, q = quit
//
// Software Requirements:
//   - C++17 compiler (g++ 7+ or clang++ 5+)
//   - OpenGL 2.1+
//   - GLUT (freeglut on Linux, built-in on macOS)
//
// =============================================================================

#ifdef __APPLE__
#  define GL_SILENCE_DEPRECATION
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glut.h>
#endif

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>

// ---------------------------------------------------------------------------
// Global State
// ---------------------------------------------------------------------------

// Window dimensions
static int winW = 1100, winH = 620;

// The eight shininess exponents displayed in the grid
static const float SHININESS_VALUES[] = { 2, 4, 8, 16, 32, 64, 128, 256 };
static const int   NUM_CUBES = 8;

// Query mode state (user enters custom shininess via terminal)
static float queryShininess = 0.0f;
static bool  queryMode      = false;

// ---------------------------------------------------------------------------
// Subdivided Face Mesh
// ---------------------------------------------------------------------------
// Each cube face is subdivided into SUBDIV × SUBDIV quads.  This gives
// (SUBDIV+1)^2 vertices per face, allowing OpenGL's per-vertex Gouraud
// shading to produce a smooth specular highlight gradient.
// Without subdivision, a flat quad has only 4 vertices and the specular
// intensity is nearly uniform — no visible highlight.
// ---------------------------------------------------------------------------
static const int SUBDIV = 80;

// Draw one face of the cube as a subdivided mesh.
// Parameters:
//   ox, oy, oz  — origin corner of the face
//   ux, uy, uz  — first edge direction (unit length, spans the face)
//   vx, vy, vz  — second edge direction (unit length, spans the face)
//   nx, ny, nz  — outward-facing unit normal
static void drawSubdividedFace(
    float ox, float oy, float oz,
    float ux, float uy, float uz,
    float vx, float vy, float vz,
    float nx, float ny, float nz)
{
    float step = 1.0f / SUBDIV;
    glNormal3f(nx, ny, nz);  // All vertices on this face share the same normal

    for (int i = 0; i < SUBDIV; ++i) {
        float u0 = i * step;
        float u1 = (i + 1) * step;
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= SUBDIV; ++j) {
            float v = j * step;
            // Vertex at (u1, v)
            glVertex3f(ox + u1*ux + v*vx,
                       oy + u1*uy + v*vy,
                       oz + u1*uz + v*vz);
            // Vertex at (u0, v)
            glVertex3f(ox + u0*ux + v*vx,
                       oy + u0*uy + v*vy,
                       oz + u0*uz + v*vz);
        }
        glEnd();
    }
}

// ---------------------------------------------------------------------------
// Draw a unit cube centered at the origin, spanning [-0.5, 0.5] on each axis.
// Each of the 6 faces is drawn as a subdivided mesh for smooth lighting.
// ---------------------------------------------------------------------------
static void drawCube()
{
    // Front face  (z = +0.5, normal = +Z)
    drawSubdividedFace(-0.5f, -0.5f,  0.5f,   1, 0, 0,   0, 1, 0,   0, 0, 1);
    // Back face   (z = -0.5, normal = -Z)
    drawSubdividedFace( 0.5f, -0.5f, -0.5f,  -1, 0, 0,   0, 1, 0,   0, 0,-1);
    // Right face  (x = +0.5, normal = +X)
    drawSubdividedFace( 0.5f, -0.5f,  0.5f,   0, 0,-1,   0, 1, 0,   1, 0, 0);
    // Left face   (x = -0.5, normal = -X)
    drawSubdividedFace(-0.5f, -0.5f, -0.5f,   0, 0, 1,   0, 1, 0,  -1, 0, 0);
    // Top face    (y = +0.5, normal = +Y)
    drawSubdividedFace(-0.5f,  0.5f,  0.5f,   1, 0, 0,   0, 0,-1,   0, 1, 0);
    // Bottom face (y = -0.5, normal = -Y)
    drawSubdividedFace(-0.5f, -0.5f, -0.5f,   1, 0, 0,   0, 0, 1,   0,-1, 0);
}

// ---------------------------------------------------------------------------
// Bitmap text rendering using GLUT
// ---------------------------------------------------------------------------
static void renderBitmapString(float x, float y, void *font, const char *s)
{
    glRasterPos2f(x, y);
    for (; *s; ++s)
        glutBitmapCharacter(font, *s);
}

// ---------------------------------------------------------------------------
// Draw one lit cube with a given shininess exponent.
// Material: copper/orange tint matching the reference image.
// ---------------------------------------------------------------------------
static void drawLitCube(float shininess)
{
    // Ambient: dark warm brown (visible in shadow areas)
    GLfloat matAmbient[]  = { 0.18f, 0.09f, 0.04f, 1.0f };
    // Diffuse: copper-orange (dominant surface color)
    GLfloat matDiffuse[]  = { 0.72f, 0.38f, 0.18f, 1.0f };
    // Specular: warm-tinted highlight (not pure white, to keep copper feel)
    GLfloat matSpecular[] = { 0.45f, 0.35f, 0.25f, 1.0f };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  matAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  matDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    drawCube();
}

// ---------------------------------------------------------------------------
// Render one cube in its own viewport grid cell.
// The cell is split: upper region for the 3D cube, lower 35px for the label.
// ---------------------------------------------------------------------------
static void renderCubeInViewport(int vx, int vy, int vw, int vh,
                                  float shininess, bool showLabel)
{
    // Reserve bottom pixels for the shininess label text
    int labelH = 35;
    int cubeVy = vy + labelH;    // Cube viewport starts above label area
    int cubeVh = vh - labelH;    // Cube viewport height (reduced)

    // --- Draw the 3D lit cube in the upper portion ---
    glViewport(vx, cubeVy, vw, cubeVh);
    glScissor(vx, cubeVy, vw, cubeVh);
    glEnable(GL_SCISSOR_TEST);
    glClearColor(0.18f, 0.18f, 0.18f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(28.0, (float)vw / (float)cubeVh, 0.1, 100.0);

    // Camera: slightly left and above, showing front face + left side + top
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(-0.85,  0.45,  2.8,    // eye position
               0.05, -0.02,  0.0,    // look-at target
               0.0,   1.0,   0.0);   // up vector

    // Positional light in front of the cube, slightly above
    GLfloat lightPos[]      = { 0.0f, 0.5f, 5.0f, 1.0f };
    GLfloat lightAmbient[]  = { 0.08f, 0.05f, 0.03f, 1.0f };
    GLfloat lightDiffuse[]  = { 0.75f, 0.60f, 0.45f, 1.0f };
    GLfloat lightSpecular[] = { 1.0f,  0.90f, 0.75f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    // Draw the cube with the specified shininess
    drawLitCube(shininess);

    // --- Draw the label in the bottom portion of the cell ---
    if (showLabel) {
        glViewport(vx, vy, vw, labelH);
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, vw, 0, labelH);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Clear label area
        glScissor(vx, vy, vw, labelH);
        glEnable(GL_SCISSOR_TEST);
        glClearColor(0.18f, 0.18f, 0.18f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);

        // White text, centered horizontally in the label strip
        glColor3f(1.0f, 1.0f, 1.0f);
        char label[32];
        snprintf(label, sizeof(label), "%g", shininess);
        float tw = (float)glutBitmapLength(GLUT_BITMAP_HELVETICA_18,
                                            (const unsigned char *)label);
        renderBitmapString((vw - tw) * 0.5f, (labelH - 14) * 0.5f,
                           GLUT_BITMAP_HELVETICA_18, label);
    }
}

// ---------------------------------------------------------------------------
// GLUT display callback — draws either the 4x2 grid or a single query cube
// ---------------------------------------------------------------------------
static void display()
{
    glViewport(0, 0, winW, winH);
    glClearColor(0.18f, 0.18f, 0.18f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!queryMode) {
        // Grid mode: 4 columns × 2 rows
        int cols = 4, rows = 2;
        int cellW = winW / cols, cellH = winH / rows;
        for (int i = 0; i < NUM_CUBES; ++i) {
            int col = i % cols;
            int row = 1 - (i / cols);  // Top row first
            renderCubeInViewport(col * cellW, row * cellH, cellW, cellH,
                                 SHININESS_VALUES[i], true);
        }
    } else {
        // Single cube mode (user-queried shininess)
        renderCubeInViewport(0, 0, winW, winH, queryShininess, true);
    }

    glutSwapBuffers();
}

// ---------------------------------------------------------------------------
// GLUT reshape callback
// ---------------------------------------------------------------------------
static void reshape(int w, int h)
{
    winW = w;
    winH = h;
    glutPostRedisplay();
}

// ---------------------------------------------------------------------------
// GLUT keyboard callback
// ---------------------------------------------------------------------------
static void keyboard(unsigned char key, int, int)
{
    if (key == 27 || key == 'q' || key == 'Q') exit(0);  // ESC or q to quit
    if (key == 'g' || key == 'G') {                       // g to return to grid
        queryMode = false;
        glutPostRedisplay();
    }
}

// ---------------------------------------------------------------------------
// Terminal-based query loop (runs via GLUT idle callback)
// After the grid is first displayed, prompts the user to enter a custom
// shininess value.  The corresponding single cube is then rendered.
// ---------------------------------------------------------------------------
static bool promptShown = false;

static void idleFunc()
{
    if (!promptShown && !queryMode) {
        promptShown = true;

        std::cout << "\n"
            << "====================================================\n"
            << "  Specular Shininess Demo — Interactive Query\n"
            << "====================================================\n"
            << "  The 4x2 grid is now displayed in the OpenGL window.\n"
            << "  Shininess values shown: 2, 4, 8, 16, 32, 64, 128, 256\n"
            << "----------------------------------------------------\n"
            << "  Enter a shininess value (e.g. 2, 50, 128, 256)\n"
            << "  to display a single cube with that exponent.\n"
            << "  Enter 0 to return to grid view.\n"
            << "  Enter 'q' to quit.\n"
            << "====================================================\n";

        while (true) {
            std::cout << "\n> Enter shininess value (0=grid, q=quit): ";
            std::string input;
            if (!std::getline(std::cin, input)) break;

            if (input == "q" || input == "Q" || input == "quit")
                exit(0);

            float val = 0;
            try {
                val = std::stof(input);
            } catch (...) {
                std::cout << "  Invalid input. Please enter a number.\n";
                continue;
            }

            if (val <= 0) {
                queryMode = false;
                glutPostRedisplay();
                std::cout << "  Grid view restored.\n";
                continue;
            }

            // OpenGL clamps shininess to [0, 128] on some implementations,
            // but most modern drivers support higher values.
            if (val > 1024) {
                val = 1024;
                std::cout << "  Clamped to 1024.\n";
            }

            queryShininess = val;
            queryMode = true;
            glutPostRedisplay();
            std::cout << "  Displaying cube with shininess = "
                      << queryShininess << "\n";
        }
    }
}

// ---------------------------------------------------------------------------
// Main entry point
// ---------------------------------------------------------------------------
int main(int argc, char **argv)
{
    std::cout << "====================================================\n"
              << "  Specular Shininess Cube Demo (OpenGL / GLUT)\n"
              << "====================================================\n"
              << "  Initializing OpenGL window...\n";

    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(winW, winH);
    glutCreateWindow("Specular Shininess Demo");

    // Global OpenGL state
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);  // Gouraud shading for smooth interpolation

    // Local viewer model: specular is computed using the true eye-to-vertex
    // direction rather than a parallel approximation — more accurate highlights
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    // Dark global ambient to keep unlit areas (sides, top) very dark
    GLfloat globalAmbient[] = { 0.05f, 0.03f, 0.02f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    // Register GLUT callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idleFunc);

    // Enter the GLUT main event loop
    glutMainLoop();
    return 0;
}