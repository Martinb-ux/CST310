// SpecularLightingDemo.cpp
// Demonstrates visual impact of different shininess values on specular lighting
// Build: make (or g++ SpecularLightingDemo.cpp -o specular_demo -lglut -lGL -lGLU)
// (On macOS: g++ SpecularLightingDemo.cpp -o specular_demo -framework OpenGL -framework GLUT)
// (On Windows: g++ SpecularLightingDemo.cpp -o specular_demo.exe -lopengl32 -lglu32 -lfreeglut)

// Platform-specific OpenGL headers
#ifdef __APPLE__
    #include <GLUT/glut.h>
#elif defined(_WIN32) || defined(_WIN64)
    #include <GL/glut.h>
#else
    // Linux and other Unix-like systems
    #include <GL/glut.h>
#endif

#include <cmath>
#include <cstdlib>
#include <vector>
#include <iostream>

// Silence deprecation warnings on macOS
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

// -----------------------------
// Camera structure
// -----------------------------
struct Camera {
    float ex, ey, ez;
    float cx, cy, cz;
    float ux, uy, uz;
};

Camera gCamera = { 0.0f, 2.5f, 9.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f };

// -----------------------------
// Cube with shininess value
// -----------------------------
struct SpecularCube {
    float shininess;
    float position[3];
    float color[3];
    const char* label;
};

std::vector<SpecularCube> gCubes;

// -----------------------------
// Initialization
// -----------------------------
void InitCubes() {
    // Create 8 cubes with different shininess values in 2x4 grid
    // Orange color matching the original reference more closely
    float orangeColor[3] = {0.9f, 0.45f, 0.1f}; // More orange, less saturated
    
    gCubes = {
        // Top row: 2, 4, 8, 16 (low to high)
        { 2.0f,   {-4.5f, 3.0f,  1.5f}, {orangeColor[0], orangeColor[1], orangeColor[2]}, "2" },
        { 4.0f,   {-1.5f, 3.0f,  1.5f}, {orangeColor[0], orangeColor[1], orangeColor[2]}, "4" },
        { 8.0f,   { 1.5f, 3.0f,  1.5f}, {orangeColor[0], orangeColor[1], orangeColor[2]}, "8" },
        { 16.0f,  { 4.5f, 3.0f,  1.5f}, {orangeColor[0], orangeColor[1], orangeColor[2]}, "16" },
        // Bottom row: 32, 64, 128, 256 (low to high)
        { 32.0f,  {-4.5f, 1.0f,  1.5f}, {orangeColor[0], orangeColor[1], orangeColor[2]}, "32" },
        { 64.0f,  {-1.5f, 1.0f,  1.5f}, {orangeColor[0], orangeColor[1], orangeColor[2]}, "64" },
        { 128.0f, { 1.5f, 1.0f,  1.5f}, {orangeColor[0], orangeColor[1], orangeColor[2]}, "128" },
        { 256.0f, { 4.5f, 1.0f,  1.5f}, {orangeColor[0], orangeColor[1], orangeColor[2]}, "256" }
    };
}

// -----------------------------
// Drawing functions
// -----------------------------
void DrawAxes(float len = 5.0f) {
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    // X (red)
    glColor3f(1,0,0); glVertex3f(0,0,0); glVertex3f(len,0,0);
    // Y (green)
    glColor3f(0,1,0); glVertex3f(0,0,0); glVertex3f(0,len,0);
    // Z (blue)
    glColor3f(0,0,1); glVertex3f(0,0,0); glVertex3f(0,0,len);
    glEnd();
    glEnable(GL_LIGHTING);
}

void DrawGround(float size = 20.0f) {
    glColor3f(0.15f, 0.15f, 0.15f); // Darker ground to match original
    glBegin(GL_QUADS);
    glVertex3f(-size, 0.0f, -size);
    glVertex3f( size, 0.0f, -size);
    glVertex3f( size, 0.0f,  size);
    glVertex3f(-size, 0.0f,  size);
    glEnd();
}

void DrawCube(float size = 1.0f) {
    float half = size * 0.5f;
    
    // Front face
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-half, -half,  half);
    glVertex3f( half, -half,  half);
    glVertex3f( half,  half,  half);
    glVertex3f(-half,  half,  half);
    glEnd();
    
    // Back face
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-half, -half, -half);
    glVertex3f(-half,  half, -half);
    glVertex3f( half,  half, -half);
    glVertex3f( half, -half, -half);
    glEnd();
    
    // Top face
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-half,  half, -half);
    glVertex3f(-half,  half,  half);
    glVertex3f( half,  half,  half);
    glVertex3f( half,  half, -half);
    glEnd();
    
    // Bottom face
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-half, -half, -half);
    glVertex3f( half, -half, -half);
    glVertex3f( half, -half,  half);
    glVertex3f(-half, -half,  half);
    glEnd();
    
    // Right face
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f( half, -half, -half);
    glVertex3f( half,  half, -half);
    glVertex3f( half,  half,  half);
    glVertex3f( half, -half,  half);
    glEnd();
    
    // Left face
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-half, -half, -half);
    glVertex3f(-half, -half,  half);
    glVertex3f(-half,  half,  half);
    glVertex3f(-half,  half, -half);
    glEnd();
}

void DrawSpecularCube(const SpecularCube& cube) {
    glPushMatrix();
    glTranslatef(cube.position[0], cube.position[1], cube.position[2]);
    
    // Disable color material to use proper material properties
    glDisable(GL_COLOR_MATERIAL);
    
    // Set material properties for this cube
    GLfloat ambient[] = { cube.color[0] * 0.3f, cube.color[1] * 0.3f, cube.color[2] * 0.3f, 1.0f };
    GLfloat diffuse[] = { cube.color[0] * 0.8f, cube.color[1] * 0.8f, cube.color[2] * 0.8f, 1.0f };
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // White specular highlight
    GLfloat shininess[] = { cube.shininess };
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    
    DrawCube(1.5f);
    
    // Re-enable color material for other elements
    glEnable(GL_COLOR_MATERIAL);
    glPopMatrix();
}

void DrawText(const char* text, float x, float y) {
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x, y);
    
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *text);
        text++;
    }
    glEnable(GL_LIGHTING);
}

// -----------------------------
// GLUT callbacks
// -----------------------------
void Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(gCamera.ex, gCamera.ey, gCamera.ez,
              gCamera.cx, gCamera.cy, gCamera.cz,
              gCamera.ux, gCamera.uy, gCamera.uz);
    
    // Draw scene elements
    DrawGround();
    DrawAxes(3.0f);
    
    // Draw cubes with different shininess values
    for (const auto& cube : gCubes) {
        DrawSpecularCube(cube);
    }
    
    // Draw labels for each cube
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    // Adjust projection for 2x4 grid arrangement
    glOrtho(-6, 6, -3, 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    for (const auto& cube : gCubes) {
        // Position labels under each cube for 2x4 grid arrangement
        float labelX = cube.position[0] - 0.3f;
        float labelY;
        
        // Top row labels (Y=3.0) and bottom row labels (Y=1.0)
        if (cube.position[1] > 2.0f) { // Top row (Y=3.0)
            labelY = -1.6f; // Closer to cubes
        } else { // Bottom row (Y=1.0)
            labelY = -2.0f; // Closer to cubes
        }
        
        DrawText(cube.label, labelX, labelY);
    }
    
    // Remove title to match reference image
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
    glutSwapBuffers();
}

void Reshape(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / (float)h;
    
    glViewport(0, 0, w, h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, aspect, 0.1, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
}

void Keyboard(unsigned char key, int, int) {
    switch (key) {
        case 27: // ESC
            std::exit(0);
            break;
        case 'a': // Move camera left
            gCamera.ex -= 0.5f;
            break;
        case 'd': // Move camera right
            gCamera.ex += 0.5f;
            break;
        case 'w': // Move camera forward
            gCamera.ez -= 0.5f;
            break;
        case 's': // Move camera backward
            gCamera.ez += 0.5f;
            break;
        case 'q': // Move camera up
            gCamera.ey += 0.5f;
            break;
        case 'e': // Move camera down
            gCamera.ey -= 0.5f;
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void InitGL() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Darker background to match original
    
    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    
    // Set up light position and properties - adjusted to match original reference
    GLfloat lightPos[] = { 1.0f, 2.0f, 4.0f, 1.0f }; // Light positioned for better specular highlights
    GLfloat lightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f }; // Lower ambient for better contrast
    GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f }; // Slightly reduced diffuse
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Strong specular
    
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    
    // Set global ambient light - darker for better contrast
    GLfloat globalAmbient[] = { 0.05f, 0.05f, 0.05f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
    
    // Enable color material for basic coloring
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("Specular Lighting Demo - Different Shininess Values");
    
    InitGL();
    InitCubes();
    
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
    
    std::cout << "=== Specular Lighting Demo ===" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  W/S - Move camera forward/backward" << std::endl;
    std::cout << "  A/D - Move camera left/right" << std::endl;
    std::cout << "  Q/E - Move camera up/down" << std::endl;
    std::cout << "  ESC - Exit" << std::endl;
    std::cout << std::endl;
    std::cout << "Each cube has a different shininess value:" << std::endl;
    std::cout << "  Orange cubes: Shininess = 2, 4, 8, 16, 32, 64, 128, 256" << std::endl;
    std::cout << "  Lower values = rough surface, Higher values = shiny surface" << std::endl;
    
    glutMainLoop();
    return 0;
}
