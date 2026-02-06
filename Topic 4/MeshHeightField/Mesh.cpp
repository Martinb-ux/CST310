// MeshHeightfield.cpp (FreeGLUT + legacy OpenGL fixed-function)
// Builds a mesh from y = f(x,z) and draws it as triangle strips.
// Controls:
//   Arrow keys: rotate
//   +/-       : zoom
//   W         : toggle wireframe
//   Esc       : quit

#include <cmath>
#include <vector>
#include <algorithm>

#ifdef _WIN32
  #include <windows.h>
#endif

#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

struct Vec3 { float x, y, z; };

static int   gWinW = 900, gWinH = 650;
static float gYaw = -55.0f, gPitch = 28.0f;
static float gZoom = 9.0f;
static bool  gWire = false;

// Grid settings
static const int   NX = 120;
static const int   NZ = 120;
static const float XMIN = -3.0f, XMAX =  3.0f;
static const float ZMIN = -3.0f, ZMAX =  3.0f;

// Function: y = f(x,z)
static float f(float x, float z) {
  return 0.8f * std::sin(x) * std::cos(z);
}

// Approximate normal from partial derivatives (central differences)
static Vec3 normalAt(float x, float z) {
  const float h = 0.01f;
  float fx1 = f(x + h, z), fx0 = f(x - h, z);
  float fz1 = f(x, z + h), fz0 = f(x, z - h);

  // Tangents along x and z directions
  Vec3 tx { 2*h, fx1 - fx0, 0.0f };
  Vec3 tz { 0.0f, fz1 - fz0, 2*h };

  // n = tz x tx  (order chosen so normals point "up" most of the time)
  Vec3 n {
    tz.y * tx.z - tz.z * tx.y,
    tz.z * tx.x - tz.x * tx.z,
    tz.x * tx.y - tz.y * tx.x
  };

  // Normalize
  float len = std::sqrt(n.x*n.x + n.y*n.y + n.z*n.z);
  if (len > 1e-6f) { n.x/=len; n.y/=len; n.z/=len; }
  return n;
}

static void setupLights() {
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  GLfloat lightPos[] = { 3.0f, 4.0f, 3.0f, 1.0f };
  GLfloat ambient[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
  GLfloat diffuse[]  = { 0.9f, 0.9f, 0.9f, 1.0f };

  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);

  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

static void drawAxes(float s=3.5f) {
  glDisable(GL_LIGHTING);
  glBegin(GL_LINES);
    glColor3f(1,0,0); glVertex3f(0,0,0); glVertex3f(s,0,0); // X
    glColor3f(0,1,0); glVertex3f(0,0,0); glVertex3f(0,s,0); // Y
    glColor3f(0,0,1); glVertex3f(0,0,0); glVertex3f(0,0,s); // Z
  glEnd();
  glEnable(GL_LIGHTING);
}

static void display() {
  glViewport(0,0,gWinW,gWinH);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Projection
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (double)gWinW/(double)gWinH, 0.1, 100.0);

  // Camera
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0f, -0.2f, -gZoom);
  glRotatef(gPitch, 1,0,0);
  glRotatef(gYaw,   0,1,0);

  drawAxes();

  glPolygonMode(GL_FRONT_AND_BACK, gWire ? GL_LINE : GL_FILL);

  // Surface color
  glColor3f(0.2f, 0.55f, 0.95f);

  // Render as triangle strips along z (each strip covers two adjacent z rows)
  float dx = (XMAX - XMIN) / (NX - 1);
  float dz = (ZMAX - ZMIN) / (NZ - 1);

  for (int j = 0; j < NZ - 1; ++j) {
    float z0 = ZMIN + j * dz;
    float z1 = ZMIN + (j + 1) * dz;

    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i < NX; ++i) {
      float x = XMIN + i * dx;

      float y0 = f(x, z0);
      Vec3 n0 = normalAt(x, z0);
      glNormal3f(n0.x, n0.y, n0.z);
      glVertex3f(x, y0, z0);

      float y1 = f(x, z1);
      Vec3 n1 = normalAt(x, z1);
      glNormal3f(n1.x, n1.y, n1.z);
      glVertex3f(x, y1, z1);
    }
    glEnd();
  }

  glutSwapBuffers();
}

static void reshape(int w, int h) {
  gWinW = std::max(1, w);
  gWinH = std::max(1, h);
}

static void keyboard(unsigned char key, int, int) {
  switch (key) {
    case 27: std::exit(0); break;      // Esc
    case 'w': case 'W': gWire = !gWire; break;
    case '+': gZoom = std::max(2.0f, gZoom - 0.5f); break;
    case '-': gZoom = std::min(30.0f, gZoom + 0.5f); break;
  }
  glutPostRedisplay();
}

static void special(int key, int, int) {
  const float step = 4.0f;
  if (key == GLUT_KEY_LEFT)  gYaw   -= step;
  if (key == GLUT_KEY_RIGHT) gYaw   += step;
  if (key == GLUT_KEY_UP)    gPitch -= step;
  if (key == GLUT_KEY_DOWN)  gPitch += step;
  glutPostRedisplay();
}

static void initGL() {
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);

  glClearColor(0.06f, 0.06f, 0.08f, 1.0f);
  setupLights();
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(gWinW, gWinH);
  glutCreateWindow("OpenGL Mesh from f(x,z) = 0.8*sin(x)*cos(z)");

  initGL();

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);

  glutMainLoop();
  return 0;
}
