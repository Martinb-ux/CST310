// Minimal SimpleMesh: heightfield y = f(x,z) rendered as triangle strips
// Controls: Arrow keys rotate, +/- zoom, Esc quit

#include <cmath>
#include <algorithm>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

static int gWinW = 800, gWinH = 600;
static float gYaw = -45.0f, gPitch = 30.0f;
static float gZoom = 8.0f;

// Grid
static const int NX = 80;
static const int NZ = 80;
static const float XMIN = -3.0f, XMAX = 3.0f;
static const float ZMIN = -3.0f, ZMAX = 3.0f;

// Height function y = f(x,z)
static float f(float x, float z) {
	return 0.6f * std::sin(2.0f * x + 1.2f * std::sin(1.5f * z))
         * std::cos(2.0f * z);
}

// Simple color map from height
static void colorFor(float y, float ymin, float ymax) {
	float t = (y - ymin) / (ymax - ymin);
	if (t < 0) t = 0; if (t > 1) t = 1;
	glColor3f(0.2f + 0.8f * t, 0.2f + 0.6f * (1.0f - t), 0.3f + 0.7f * (1.0f - t));
}

static void display() {
	glViewport(0,0,gWinW,gWinH);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)gWinW / (double)gWinH, 0.1, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, -0.5f, -gZoom);
	glRotatef(gPitch, 1,0,0);
	glRotatef(gYaw,   0,1,0);

	// find min/max y
	float ymin = 1e9f, ymax = -1e9f;
	for (int j = 0; j < NZ; ++j) {
		float z = ZMIN + (ZMAX - ZMIN) * j / (NZ - 1);
		for (int i = 0; i < NX; ++i) {
			float x = XMIN + (XMAX - XMIN) * i / (NX - 1);
			float y = f(x,z);
			ymin = std::min(ymin, y);
			ymax = std::max(ymax, y);
		}
	}

	glDisable(GL_LIGHTING);
	glBegin(GL_TRIANGLE_STRIP);
	for (int j = 0; j < NZ - 1; ++j) {
		float z0 = ZMIN + (ZMAX - ZMIN) * j / (NZ - 1);
		float z1 = ZMIN + (ZMAX - ZMIN) * (j+1) / (NZ - 1);
		for (int i = 0; i < NX; ++i) {
			float x = XMIN + (XMAX - XMIN) * i / (NX - 1);
			float y0 = f(x, z0);
			colorFor(y0, ymin, ymax);
			glVertex3f(x, y0, z0);
			float y1 = f(x, z1);
			colorFor(y1, ymin, ymax);
			glVertex3f(x, y1, z1);
		}
	}
	glEnd();

	glutSwapBuffers();
}

static void reshape(int w, int h) {
	gWinW = std::max(1, w);
	gWinH = std::max(1, h);
}

static void keyboard(unsigned char key, int, int) {
	switch (key) {
		case 27: std::exit(0); break;
		case '+': gZoom = std::max(1.0f, gZoom - 0.5f); break;
		case '-': gZoom = std::min(50.0f, gZoom + 0.5f); break;
	}
	glutPostRedisplay();
}

static void special(int key, int, int) {
	const float step = 5.0f;
	if (key == GLUT_KEY_LEFT)  gYaw   -= step;
	if (key == GLUT_KEY_RIGHT) gYaw   += step;
	if (key == GLUT_KEY_UP)    gPitch -= step;
	if (key == GLUT_KEY_DOWN)  gPitch += step;
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(gWinW, gWinH);
	glutCreateWindow("SimpleMesh: y = 0.6*sin(2x + 1.2*sin(1.5z)) * cos(2z)");

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.1f, 0.12f, 1.0f);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMainLoop();
	return 0;
}