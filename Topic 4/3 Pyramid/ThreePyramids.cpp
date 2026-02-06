// ThreePyramids.cpp
// Build (macOS example): g++ ThreePyramids_3Views.cpp -framework OpenGL -framework GLUT -o ThreePyramids_3Views
// Build (Linux example): g++ ThreePyramids_3Views.cpp -lGL -lGLU -lglut -o ThreePyramids_3Views
// Controls: 1/2/3 switch cameras, ESC quits

#include <cstdlib>
#include <cmath>

#ifdef _WIN32
  #include <windows.h>
#endif

#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

struct Camera {
  float eye[3];
  float center[3];
  float up[3];
};

int gWidth = 900, gHeight = 600;
int gActiveCam = 0;

// Three different perspectives
Camera gCams[3] = {
  // View 1: Front-ish centered (classic)
  { { 0.0f, 2.0f, 10.0f }, { 0.0f, 0.8f, 0.0f }, { 0.0f, 1.0f, 0.0f } },

  // View 2: Left side, looking across X axis (emphasizes “one next to another”)
  { { -10.0f, 2.0f, 3.0f }, { 0.0f, 0.8f, 0.0f }, { 0.0f, 1.0f, 0.0f } },

  // View 3: Higher + angled (top/diagonal perspective)
  { { 5.0f, 7.0f, 8.0f }, { 0.0f, 0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f } }
};

void setupProjection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (double)gWidth / (double)gHeight, 0.1, 100.0);
  glMatrixMode(GL_MODELVIEW);
}

void applyCamera() {
  const Camera& c = gCams[gActiveCam];
  glLoadIdentity();
  gluLookAt(
    c.eye[0], c.eye[1], c.eye[2],
    c.center[0], c.center[1], c.center[2],
    c.up[0], c.up[1], c.up[2]
  );
}

void drawGroundGrid(float size = 12.0f, float step = 1.0f) {
  glDisable(GL_LIGHTING);
  glColor3f(0.25f, 0.25f, 0.25f);
  glBegin(GL_LINES);
  for (float i = -size; i <= size; i += step) {
    glVertex3f(i, 0.0f, -size); glVertex3f(i, 0.0f,  size);
    glVertex3f(-size, 0.0f, i); glVertex3f( size, 0.0f,  i);
  }
  glEnd();

  // Axes
  glBegin(GL_LINES);
  // X axis (red)
  glColor3f(1.0f, 0.2f, 0.2f);
  glVertex3f(-size, 0.01f, 0.0f); glVertex3f(size, 0.01f, 0.0f);
  // Z axis (blue)
  glColor3f(0.2f, 0.2f, 1.0f);
  glVertex3f(0.0f, 0.01f, -size); glVertex3f(0.0f, 0.01f, size);
  glEnd();
}

void drawPyramid(float halfBase = 0.8f, float height = 1.6f) {
  // Vertices
  const float y0 = 0.0f;
  const float yTop = height;

  // Base corners (square)
  float v0[3] = { -halfBase, y0, -halfBase };
  float v1[3] = {  halfBase, y0, -halfBase };
  float v2[3] = {  halfBase, y0,  halfBase };
  float v3[3] = { -halfBase, y0,  halfBase };

  // Apex
  float top[3] = { 0.0f, yTop, 0.0f };

  // Sides (4 triangles)
  glBegin(GL_TRIANGLES);

  // Front face (v3, v2, top)
  glColor3f(0.9f, 0.7f, 0.2f);
  glVertex3fv(v3); glVertex3fv(v2); glVertex3fv(top);

  // Right face (v2, v1, top)
  glColor3f(0.2f, 0.8f, 0.7f);
  glVertex3fv(v2); glVertex3fv(v1); glVertex3fv(top);

  // Back face (v1, v0, top)
  glColor3f(0.8f, 0.3f, 0.7f);
  glVertex3fv(v1); glVertex3fv(v0); glVertex3fv(top);

  // Left face (v0, v3, top)
  glColor3f(0.3f, 0.7f, 0.3f);
  glVertex3fv(v0); glVertex3fv(v3); glVertex3fv(top);

  glEnd();

  // Base (2 triangles)
  glBegin(GL_TRIANGLES);
  glColor3f(0.6f, 0.6f, 0.6f);
  glVertex3fv(v0); glVertex3fv(v1); glVertex3fv(v2);
  glVertex3fv(v0); glVertex3fv(v2); glVertex3fv(v3);
  glEnd();
}

void drawScene() {
  drawGroundGrid();

  // Three pyramids parallel to the x-axis (left, center, right)
  float xs[3] = { -2.5f, 0.0f, 2.5f };

  for (int i = 0; i < 3; i++) {
    glPushMatrix();
    glTranslatef(xs[i], 0.0f, 0.0f);
    drawPyramid();
    glPopMatrix();
  }
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  applyCamera();
  drawScene();

  glutSwapBuffers();
}

void reshape(int w, int h) {
  gWidth = (w > 1) ? w : 1;
  gHeight = (h > 1) ? h : 1;
  glViewport(0, 0, gWidth, gHeight);
  setupProjection();
}

void keyboard(unsigned char key, int, int) {
  switch (key) {
    case 27: // ESC
      std::exit(0);
      break;
    case '1':
      gActiveCam = 0;
      glutPostRedisplay();
      break;
    case '2':
      gActiveCam = 1;
      glutPostRedisplay();
      break;
    case '3':
      gActiveCam = 2;
      glutPostRedisplay();
      break;
    default:
      break;
  }
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(gWidth, gHeight);
  glutCreateWindow("Three Pyramids - 3 Camera Views (1/2/3)");

  glEnable(GL_DEPTH_TEST);
  glClearColor(0.08f, 0.08f, 0.10f, 1.0f);

  setupProjection();

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);

  glutMainLoop();
  return 0;
}