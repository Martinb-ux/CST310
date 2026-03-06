// ColorCubeFlyby.cpp
// Part 2 (updated full code):
//  - r : toggle rotation of the whole scene
//  - s : stop (pause camera flyby + cube motion)
//  - c : continue (resume camera flyby + cube motion)
//  - u/d : move whole scene up/down (works even while paused)
//  - +/- : zoom in/out
//  - multiple cubes with different colors + lighting/brightness
//  - left/right vertical planes and cube bouncing between them
//  - KEEP the tumbling effect, but make walls dynamically sized so BOTH walls
//    always have some visible portion in frame.
//
// Note (macOS): OpenGL/GLUT are deprecated. We silence warnings for class work.

#ifdef __APPLE__
#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cmath>
#include <cstdlib>
#include <iostream>

// -----------------------------
// Original RGB color cube data
// -----------------------------
namespace Cube {
const int NUM_VERTICES = 8;
const int NUM_FACES = 6;

GLint vertices[NUM_VERTICES][3] = {
  {0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1},
  {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}
};

GLint faces[NUM_FACES][4] = {
  {1, 5, 7, 3}, {5, 4, 6, 7}, {4, 0, 2, 6},
  {3, 7, 6, 2}, {0, 1, 3, 2}, {0, 4, 5, 1}
};

GLfloat vertexColors[NUM_VERTICES][3] = {
  {0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 1.0},
  {1.0, 0.0, 0.0}, {1.0, 0.0, 1.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 1.0}
};

// Draw original RGB color cube (classic vertex-color look)
void drawRGBColorCube() {
  glBegin(GL_QUADS);
  for (int i = 0; i < NUM_FACES; i++) {
    for (int j = 0; j < 4; j++) {
      glColor3fv((GLfloat*)&vertexColors[faces[i][j]]);
      glVertex3iv((GLint*)&vertices[faces[i][j]]);
    }
  }
  glEnd();
}
} // namespace Cube

// -----------------------------
// Scene state + controls
// -----------------------------
static bool  g_paused      = false; // s = pause, c = continue
static bool  g_rotateScene = false; // r toggles rotation
static float g_sceneRotDeg = 0.0f;  // scene rotation angle
static float g_sceneY      = 0.0f;  // move whole scene up/down
static float g_zoom        = 1.0f;  // zoom in/out

// Flyby camera parameter (time)
static float g_u = 0.0f;

// View/projection tracking (for dynamic wall sizing)
static float g_fovyDeg = 75.0f; // feel free to change, used in reshape + wall sizing
static float g_aspect  = 1.0f;  // updated in reshape()

// Timer settings
static const int   TIMER_MS      = 16;   // ~60 FPS
static const float CAMERA_STEP   = 0.01f;
static const float ROT_STEP_DEG  = 1.0f;

// Bounce planes (walls)
static const float PLANE_LEFT_X  = -4.0f;
static const float PLANE_RIGHT_X =  4.0f;

// -----------------------------
// A simple bouncing cube instance
// -----------------------------
struct MovingCube {
  float x, y, z;       // position
  float vx;            // x velocity
  float size;          // cube size

  // color + "illumination/brightness" feel
  float r, g, b;
  float brightness;    // multiplies diffuse
  float shininess;     // specular exponent
};

static MovingCube g_cubes[] = {
  // x,    y,    z,     vx,   size,   r,    g,    b,   brightness, shininess
  {-2.5f, 0.3f, 0.0f,  1.2f, 0.7f,  1.0f, 0.2f, 0.2f, 1.00f, 32.0f}, // bright red-ish
  { 0.0f,-0.6f,-0.5f, -1.6f, 0.9f,  0.2f, 1.0f, 0.3f, 0.65f,  8.0f}, // dimmer green-ish
  { 2.2f, 0.8f, 0.7f,  2.0f, 0.6f,  0.3f, 0.4f, 1.0f, 1.25f, 64.0f}  // brighter blue-ish
};
static const int NUM_CUBES = sizeof(g_cubes) / sizeof(g_cubes[0]);

// -----------------------------
// Utility: print current state
// -----------------------------
static void printState() {
  std::cout << "State => paused=" << (g_paused ? "true" : "false")
            << ", rotate=" << (g_rotateScene ? "true" : "false")
            << ", rotDeg=" << g_sceneRotDeg
            << ", sceneY=" << g_sceneY
            << ", zoom=" << g_zoom
            << ", fovy=" << g_fovyDeg
            << ", aspect=" << g_aspect
            << std::endl;
}

// -----------------------------
// Lighting setup
// -----------------------------
static void setupLighting() {
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE); // keep normals correct after scaling

  // Light position (w=1 => positional light)
  GLfloat lightPos[] = { 5.0f, 6.0f, 7.0f, 1.0f };
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

  // Light colors
  GLfloat ambient[]  = { 0.20f, 0.20f, 0.20f, 1.0f };
  GLfloat diffuse[]  = { 0.95f, 0.95f, 0.95f, 1.0f };
  GLfloat specular[] = { 0.90f, 0.90f, 0.90f, 1.0f };
  glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}

// -----------------------------
// Draw a cube centered at origin with normals (for lighting)
// -----------------------------
static void drawLitCube(float size, float r, float g, float b, float brightness, float shininess) {
  const float hs = size * 0.5f;

  // Material: diffuse scaled by brightness
  GLfloat matAmbient[]  = { 0.15f * r, 0.15f * g, 0.15f * b, 1.0f };
  GLfloat matDiffuse[]  = { brightness * r, brightness * g, brightness * b, 1.0f };
  GLfloat matSpecular[] = { 0.7f, 0.7f, 0.7f, 1.0f };

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  matAmbient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  matDiffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

  glBegin(GL_QUADS);

  // +X face
  glNormal3f(1, 0, 0);
  glVertex3f( hs, -hs, -hs);
  glVertex3f( hs,  hs, -hs);
  glVertex3f( hs,  hs,  hs);
  glVertex3f( hs, -hs,  hs);

  // -X face
  glNormal3f(-1, 0, 0);
  glVertex3f(-hs, -hs,  hs);
  glVertex3f(-hs,  hs,  hs);
  glVertex3f(-hs,  hs, -hs);
  glVertex3f(-hs, -hs, -hs);

  // +Y face
  glNormal3f(0, 1, 0);
  glVertex3f(-hs,  hs, -hs);
  glVertex3f(-hs,  hs,  hs);
  glVertex3f( hs,  hs,  hs);
  glVertex3f( hs,  hs, -hs);

  // -Y face
  glNormal3f(0, -1, 0);
  glVertex3f(-hs, -hs,  hs);
  glVertex3f(-hs, -hs, -hs);
  glVertex3f( hs, -hs, -hs);
  glVertex3f( hs, -hs,  hs);

  // +Z face
  glNormal3f(0, 0, 1);
  glVertex3f(-hs, -hs,  hs);
  glVertex3f( hs, -hs,  hs);
  glVertex3f( hs,  hs,  hs);
  glVertex3f(-hs,  hs,  hs);

  // -Z face
  glNormal3f(0, 0, -1);
  glVertex3f(-hs,  hs, -hs);
  glVertex3f( hs,  hs, -hs);
  glVertex3f( hs, -hs, -hs);
  glVertex3f(-hs, -hs, -hs);

  glEnd();
}

// -----------------------------
// Fixed large walls that always stay visible with consistent appearance
// -----------------------------
static void drawWallsDynamic() {
  // Lighter wall material - less opaque appearance
  GLfloat wallAmbient[]  = { 0.4f, 0.4f, 0.45f, 1.0f };
  GLfloat wallDiffuse[]  = { 0.7f, 0.7f, 0.8f, 1.0f };
  GLfloat wallSpecular[] = { 0.05f, 0.05f, 0.05f, 1.0f };
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  wallAmbient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  wallDiffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, wallSpecular);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 4.0f);

  // Fixed large size - big enough to always stay visible regardless of camera angle
  const float y0 = -8.0f, y1 = 8.0f;
  const float z0 = -8.0f, z1 = 8.0f;

  glBegin(GL_QUADS);

  // Left wall (facing +X)
  glNormal3f(1, 0, 0);
  glVertex3f(PLANE_LEFT_X,  y0, z0);
  glVertex3f(PLANE_LEFT_X,  y1, z0);
  glVertex3f(PLANE_LEFT_X,  y1, z1);
  glVertex3f(PLANE_LEFT_X,  y0, z1);

  // Right wall (facing -X)
  glNormal3f(-1, 0, 0);
  glVertex3f(PLANE_RIGHT_X, y0, z1);
  glVertex3f(PLANE_RIGHT_X, y1, z1);
  glVertex3f(PLANE_RIGHT_X, y1, z0);
  glVertex3f(PLANE_RIGHT_X, y0, z0);

  glEnd();
}

// -----------------------------
// display()
// Draw everything with depth + lighting
// -----------------------------
void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Camera position from flyby curve
  float camX = 8.0f * std::cos(g_u);
  float camY = 7.0f * std::cos(g_u) - 1.0f;
  float camZ = 4.0f * std::cos(g_u / 3.0f) + 2.0f;

  // Camera look-at (KEEP tumbling effect by varying the up vector)
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(camX, camY, camZ,
            0.5, 0.5, 0.5,
            std::cos(g_u), 1, 0);

  // Apply whole-scene transforms (move/zoom/optional rotation)
  glPushMatrix();
  glTranslatef(0.0f, g_sceneY, 0.0f);
  glScalef(g_zoom, g_zoom, g_zoom);
  if (g_rotateScene) {
    glRotatef(g_sceneRotDeg, 0.0f, 1.0f, 0.0f); // rotate around Y axis
  }

  // Draw dynamic walls first so they are always visible reference planes
  drawWallsDynamic();

  // Draw original RGB cube (unlit vertex colors)
  // Temporarily disable lighting so vertex colors show clearly.
  glDisable(GL_LIGHTING);
  glPushMatrix();
    glTranslatef(-0.5f, -0.5f, -0.5f); // recenter original (0..1) cube around origin
    Cube::drawRGBColorCube();
  glPopMatrix();
  glEnable(GL_LIGHTING);

  // Draw added lit cubes (bouncing)
  for (int i = 0; i < NUM_CUBES; i++) {
    glPushMatrix();
      glTranslatef(g_cubes[i].x, g_cubes[i].y, g_cubes[i].z);
      drawLitCube(g_cubes[i].size,
                  g_cubes[i].r, g_cubes[i].g, g_cubes[i].b,
                  g_cubes[i].brightness,
                  g_cubes[i].shininess);
    glPopMatrix();
  }

  glPopMatrix();

  glutSwapBuffers();
}

// -----------------------------
// timer()
// Updates camera + cubes unless paused, and updates rotation if enabled
// -----------------------------
void timer(int /*v*/) {
  if (!g_paused) {
    // Flyby camera motion
    g_u += CAMERA_STEP;

    // Cube bouncing between vertical planes
    const float dt = TIMER_MS / 1000.0f;

    for (int i = 0; i < NUM_CUBES; i++) {
      g_cubes[i].x += g_cubes[i].vx * dt;

      // Bounce check (use half-size so cube doesn't go through wall)
      float hs = g_cubes[i].size * 0.5f;
      if (g_cubes[i].x + hs > PLANE_RIGHT_X) {
        g_cubes[i].x = PLANE_RIGHT_X - hs;
        g_cubes[i].vx *= -1.0f;
      }
      if (g_cubes[i].x - hs < PLANE_LEFT_X) {
        g_cubes[i].x = PLANE_LEFT_X + hs;
        g_cubes[i].vx *= -1.0f;
      }
    }
  }

  // Whole-scene rotation toggled by 'r'
  if (g_rotateScene) {
    g_sceneRotDeg += ROT_STEP_DEG;
    if (g_sceneRotDeg >= 360.0f) g_sceneRotDeg -= 360.0f;
  }

  glutPostRedisplay();
  glutTimerFunc(TIMER_MS, timer, 0);
}

// -----------------------------
// reshape()
// Sets viewport + projection and stores aspect for wall sizing.
// -----------------------------
void reshape(int w, int h) {
  if (h == 0) h = 1;

  g_aspect = (float)w / (float)h;

  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Use g_fovyDeg so wall sizing math matches what the camera sees
  gluPerspective(g_fovyDeg, g_aspect, 0.5, 60.0);

  glMatrixMode(GL_MODELVIEW);
}

// -----------------------------
// keyboard()
// Prints key to terminal and applies actions.
// -----------------------------
void keyboard(unsigned char key, int /*x*/, int /*y*/) {
  std::cout << "Key pressed: '" << key << "' (ASCII " << (int)key << ")\n";

  switch (key) {
    case 'r':
    case 'R':
      g_rotateScene = !g_rotateScene;
      std::cout << "Action: toggle rotation => " << (g_rotateScene ? "ON" : "OFF") << "\n";
      break;

    case 's':
    case 'S':
      g_paused = true;
      std::cout << "Action: STOP (pause flyby + cube motion)\n";
      break;

    case 'c':
    case 'C':
      g_paused = false;
      std::cout << "Action: CONTINUE (resume flyby + cube motion)\n";
      break;

    case 'u':
    case 'U':
      g_sceneY += 0.2f;
      std::cout << "Action: move scene UP\n";
      break;

    case 'd':
    case 'D':
      g_sceneY -= 0.2f;
      std::cout << "Action: move scene DOWN\n";
      break;

    case '+':
    case '=': // helps if '+' requires shift
      g_zoom *= 1.1f;
      if (g_zoom > 8.0f) g_zoom = 8.0f;
      std::cout << "Action: ZOOM IN\n";
      break;

    case '-':
    case '_':
      g_zoom /= 1.1f;
      if (g_zoom < 0.2f) g_zoom = 0.2f;
      std::cout << "Action: ZOOM OUT\n";
      break;

    case 27: // ESC
      std::cout << "Action: quit\n";
      std::exit(0);
      break;

    default:
      std::cout << "Action: (no binding)\n";
      break;
  }

  printState();
  glutPostRedisplay();
}

// -----------------------------
// init()
// Enables culling, depth buffer, and lighting.
// -----------------------------
void init() {
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  // Depth test is needed now that we have multiple objects + planes
  glEnable(GL_DEPTH_TEST);

  setupLighting();

  // Background color
  glClearColor(0.06f, 0.06f, 0.08f, 1.0f);
}

// -----------------------------
// main()
// -----------------------------
int main(int argc, char** argv) {
  glutInit(&argc, argv);

  // Depth buffer added for multiple objects/planes
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  glutInitWindowSize(500, 500);
  glutCreateWindow("Color Cube Flyby - Part 2 Controls: r s/c u/d +/-");

  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutTimerFunc(TIMER_MS, timer, 0);

  init();
  glutMainLoop();
  return 0;
}