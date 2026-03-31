#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>

const float PI = 3.14159f;

void drawSphere(float r, int stacks, int slices) {
    for (int i = 0; i < stacks; i++) {
        float phi1 = PI * i / stacks;
        float phi2 = PI * (i + 1) / stacks;

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; j++) {
            float theta = 2 * PI * j / slices;

            float x1 = r * sin(phi1) * cos(theta);
            float y1 = r * cos(phi1);
            float z1 = r * sin(phi1) * sin(theta);

            float x2 = r * sin(phi2) * cos(theta);
            float y2 = r * cos(phi2);
            float z2 = r * sin(phi2) * sin(theta);

            glVertex3f(x1, y1, z1);
            glVertex3f(x2, y2, z2);
        }
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -5.0f);
    glRotatef(30, 1, 0, 0);
    glRotatef(30, 0, 1, 0);

    drawSphere(1.5f, 20, 20);

    glutSwapBuffers();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(60, 1, 1, 20);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Procedural Sphere");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}