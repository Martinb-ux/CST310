#include <GL/glut.h>
#include <cmath>

// Light properties
GLfloat light_position[] = {2.0f, 2.0f, 2.0f, 1.0f};
GLfloat light_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

// Material properties (demonstrates how illumination affects different materials)
GLfloat material_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
GLfloat material_diffuse[] = {0.8f, 0.4f, 0.4f, 1.0f};  // Red diffuse
GLfloat material_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat material_shininess[] = {50.0f};

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);  // Enable illumination calculations
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    
    // Set up light properties (illumination)
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    // Set up material properties (how surface responds to illumination)
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);
    
    // Enable smooth shading (Gouraud shading)
    glShadeModel(GL_SMOOTH);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Set camera position
    gluLookAt(3.0, 3.0, 5.0,  // Eye position
              0.0, 0.0, 0.0,  // Look at point
              0.0, 1.0, 0.0); // Up vector
    
    // Draw a sphere to demonstrate illumination and shading
    glutSolidSphere(1.0, 32, 32);
    
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Illumination and Shading Example");
    
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    
    glutMainLoop();
    return 0;
}
