// ThreePyramids_3Cameras.cpp
// Build (example):
// g++ ThreePyramids_3Cameras.cpp -o app -lglut -lGL -lGLU
// (On Windows with FreeGLUT: link against freeglut, opengl32, glu32)

#include <GLUT/glut.h>
#include <cmath>
#include <cstdlib>

// -----------------------------
// Camera presets (eye, center, up)
// -----------------------------
struct Camera {
    float ex, ey, ez;
    float cx, cy, cz;
    float ux, uy, uz;
};

Camera gCams[3] = {
    // View 1: front-ish view (looking toward -Z)
    {  0.0f,  1.8f,  8.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f },
    // View 2: left side angle (looking across +X toward center)
    { -8.0f,  2.2f,  5.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f },
    // View 3: higher “top-ish” diagonal view
    {  0.0f,  8.0f,  8.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f }
};

int gCamIndex = 0;

// -----------------------------
// Simple helpers
// -----------------------------
void DrawAxes(float len = 5.0f) {
    glBegin(GL_LINES);
    // X (red)
    glColor3f(1,0,0); glVertex3f(0,0,0); glVertex3f(len,0,0);
    // Y (green)
    glColor3f(0,1,0); glVertex3f(0,0,0); glVertex3f(0,len,0);
    // Z (blue)
    glColor3f(0,0,1); glVertex3f(0,0,0); glVertex3f(0,0,len);
    glEnd();
}

void DrawGround(float size = 20.0f) {
    glColor3f(0.85f, 0.85f, 0.85f);
    glBegin(GL_QUADS);
    glVertex3f(-size, 0.0f, -size);
    glVertex3f( size, 0.0f, -size);
    glVertex3f( size, 0.0f,  size);
    glVertex3f(-size, 0.0f,  size);
    glEnd();
}

// Pyramid with square base centered at origin on y=0, apex at (0,height,0)
void DrawPyramid(float baseHalf = 1.0f, float height = 1.8f) {
    // vertices
    float x = baseHalf, z = baseHalf;
    // base corners (y=0)
    float v0[3] = { -x, 0.0f, -z };
    float v1[3] = {  x, 0.0f, -z };
    float v2[3] = {  x, 0.0f,  z };
    float v3[3] = { -x, 0.0f,  z };
    // apex
    float a[3]  = {  0.0f, height, 0.0f };

    // Base (optional) - darker
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
    glVertex3fv(v0); glVertex3fv(v1); glVertex3fv(v2); glVertex3fv(v3);
    glEnd();

    // Sides (4 triangles) - each face different shade for depth cues
    glBegin(GL_TRIANGLES);

    glColor3f(0.95f, 0.65f, 0.20f); // face 1
    glVertex3fv(v0); glVertex3fv(v1); glVertex3fv(a);

    glColor3f(0.85f, 0.55f, 0.20f); // face 2
    glVertex3fv(v1); glVertex3fv(v2); glVertex3fv(a);

    glColor3f(0.75f, 0.45f, 0.20f); // face 3
    glVertex3fv(v2); glVertex3fv(v3); glVertex3fv(a);

    glColor3f(0.65f, 0.35f, 0.20f); // face 4
    glVertex3fv(v3); glVertex3fv(v0); glVertex3fv(a);

    glEnd();
}

void ApplyCamera() {
    const Camera &c = gCams[gCamIndex];
    gluLookAt(c.ex, c.ey, c.ez,
              c.cx, c.cy, c.cz,
              c.ux, c.uy, c.uz);
}

// -----------------------------
// GLUT callbacks
// -----------------------------
void Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    ApplyCamera();

    // scene
    DrawGround();
    DrawAxes(3.0f);

    // 3 pyramids along X axis: x = -3, 0, +3
    glPushMatrix();
        glTranslatef(-3.0f, 0.0f, 0.0f);
        DrawPyramid(1.0f, 1.8f);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.0f);
        DrawPyramid(1.0f, 1.8f);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(3.0f, 0.0f, 0.0f);
        DrawPyramid(1.0f, 1.8f);
    glPopMatrix();

    glutSwapBuffers();
}

void Reshape(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / (float)h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Perspective projection (fov, aspect, near, far)
    gluPerspective(60.0, aspect, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

void Keyboard(unsigned char key, int, int) {
    switch (key) {
        case 27: // ESC
            std::exit(0);
            break;
        case '1':
            gCamIndex = 0;
            break;
        case '2':
            gCamIndex = 1;
            break;
        case '3':
            gCamIndex = 2;
            break;
        case ' ': // cycle
            gCamIndex = (gCamIndex + 1) % 3;
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void InitGL() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.10f, 0.10f, 0.12f, 1.0f);

    // Basic lighting (optional but helps depth)
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float lightPos[] = { 3.0f, 6.0f, 6.0f, 1.0f };
    float ambient[]  = { 0.25f, 0.25f, 0.25f, 1.0f };
    float diffuse[]  = { 0.90f, 0.90f, 0.90f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);

    // Color material lets glColor affect lighting
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(900, 600);
    glutCreateWindow("Three Pyramids - 3 Camera Views (1/2/3)");

    InitGL();

    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);

    glutMainLoop();
    return 0;
}