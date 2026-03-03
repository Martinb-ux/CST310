// CheckeredTriangles.cpp
// Textured triangles + spinning + keyboard controls
//
// Controls:
//   p = pause spinning
//   c = continue spinning
//   u/d = move up/down
//   L/R (or l/r) = move left/right
//   + / - = zoom in / zoom out
//   Esc = quit
//
// Note: On macOS, OpenGL/GLUT is deprecated. We silence those warnings using
//       GL_SILENCE_DEPRECATION (this is normal for class GLUT projects).

#ifdef __APPLE__
// Must be defined BEFORE including OpenGL/GLUT headers to silence deprecation warnings
#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cstdlib>
#include <iostream>

// -----------------------------
// Texture data (2x2 checkered)
// -----------------------------
#define red     {0xff, 0x00, 0x00}
#define yellow  {0xff, 0xff, 0x00}

GLubyte texture[][3] = {
    red, yellow,
    yellow, red,
};

// -----------------------------
// Animation / Transform State
// -----------------------------
static bool  g_spinning = true;  // whether rotation updates
static float g_angleDeg = 0.0f;  // current rotation angle in degrees (0..360)
static float g_posX     = 0.0f;  // translation left/right
static float g_posY     = 0.0f;  // translation up/down
static float g_zoom     = 1.0f;  // uniform scale

static const float ROT_STEP_DEG = 1.0f; // degrees per timer tick
static const int   TIMER_MS     = 16;   // ~60 FPS

// -----------------------------
// reshape()
// Sets viewport, projection, camera, and loads the texture.
// Called whenever the window is resized.
// -----------------------------
void reshape(int width, int height) {
  if (height == 0) height = 1;

  glViewport(0, 0, width, height);

  // Projection
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(80.0, (GLfloat)width / (GLfloat)height, 1.0, 40.0);

  // ModelView (camera)
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(2, -1, 5,   0, 0, 0,   0, 1, 0);

  // Texture setup
  glEnable(GL_TEXTURE_2D);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D,
               0,                // level 0
               3,                // RGB components
               2, 2,             // width, height
               0,                // border
               GL_RGB,           // format
               GL_UNSIGNED_BYTE, // type
               texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

// -----------------------------
// display()
// Clears the screen, applies transforms, draws triangles, swaps buffers.
// -----------------------------
void display() {
  glClear(GL_COLOR_BUFFER_BIT);

  // Reset camera each frame
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(2, -1, 5,   0, 0, 0,   0, 1, 0);

  // Apply transforms in a protected matrix scope
  glPushMatrix();

  // Move, zoom, then rotate about Z-axis
  glTranslatef(g_posX, g_posY, 0.0f);
  glScalef(g_zoom, g_zoom, 1.0f);
  glRotatef(g_angleDeg, 0.0f, 0.0f, 1.0f);

  // Draw three textured triangles
  glBegin(GL_TRIANGLES);
    glTexCoord2f(0.5f, 1.0f);  glVertex2f(-3.0f,  3.0f);
    glTexCoord2f(0.0f, 0.0f);  glVertex2f(-3.0f,  0.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex2f( 0.0f,  0.0f);

    glTexCoord2f(4.0f, 8.0f);  glVertex2f( 3.0f,  3.0f);
    glTexCoord2f(0.0f, 0.0f);  glVertex2f( 0.0f,  0.0f);
    glTexCoord2f(8.0f, 0.0f);  glVertex2f( 3.0f,  0.0f);

    glTexCoord2f(5.0f, 5.0f);  glVertex2f( 0.0f,  0.0f);
    glTexCoord2f(0.0f, 0.0f);  glVertex2f(-1.5f, -3.0f);
    glTexCoord2f(4.0f, 0.0f);  glVertex2f( 1.5f, -3.0f);
  glEnd();

  glPopMatrix();

  glutSwapBuffers();
}

// -----------------------------
// timer()
// Called repeatedly; updates rotation if spinning.
// -----------------------------
void timer(int /*value*/) {
  if (g_spinning) {
    g_angleDeg += ROT_STEP_DEG;

    // keep angle within [0, 360)
    if (g_angleDeg >= 360.0f) g_angleDeg -= 360.0f;
  }

  glutPostRedisplay();
  glutTimerFunc(TIMER_MS, timer, 0);
}

// Helper: print current transform state
static void printState() {
  std::cout << "State => spinning=" << (g_spinning ? "true" : "false")
            << ", angle=" << g_angleDeg
            << ", pos=(" << g_posX << ", " << g_posY << ")"
            << ", zoom=" << g_zoom
            << std::endl;
}

// -----------------------------
// keyboard()
// Handles key presses + prints what happened.
// -----------------------------
void keyboard(unsigned char key, int /*x*/, int /*y*/) {
  std::cout << "Key pressed: '" << key << "' (ASCII " << (int)key << ")\n";

  switch (key) {
    case 'p':
    case 'P':
      g_spinning = false;
      std::cout << "Action: pause spinning\n";
      break;

    case 'c':
    case 'C':
      g_spinning = true;
      std::cout << "Action: continue spinning\n";
      break;

    case 'u':
    case 'U':
      g_posY += 0.2f;
      std::cout << "Action: move up\n";
      break;

    case 'd':
    case 'D':
      g_posY -= 0.2f;
      std::cout << "Action: move down\n";
      break;

    case 'L':
    case 'l':
      g_posX -= 0.2f;
      std::cout << "Action: move left\n";
      break;

    case 'R':
    case 'r':
      g_posX += 0.2f;
      std::cout << "Action: move right\n";
      break;

    case '+':
    case '=': // helps on keyboards where '+' requires shift
      g_zoom *= 1.1f;
      if (g_zoom > 10.0f) g_zoom = 10.0f;
      std::cout << "Action: zoom in\n";
      break;

    case '-':
    case '_':
      g_zoom /= 1.1f;
      if (g_zoom < 0.1f) g_zoom = 0.1f;
      std::cout << "Action: zoom out\n";
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
// main()
// Initializes GLUT + starts the animation loop.
// -----------------------------
int main(int argc, char** argv) {
  glutInit(&argc, argv);

  // Double buffering for smooth animation
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  glutInitWindowSize(520, 390);
  glutCreateWindow("Textured Triangles - p/c u/d L/R +/-");

  // Set a dark background color
  glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);

  // Start timer loop
  glutTimerFunc(TIMER_MS, timer, 0);

  glutMainLoop();
  return 0;
}