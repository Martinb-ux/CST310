
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <fstream>
#include <vector>

std::vector<std::vector<std::pair<GLfloat, GLfloat>>> polylines;
GLfloat dinosaurPosition = 0.0f;  
GLfloat legAngle = 0.0f;          
GLfloat direction = 1.0f;         
GLfloat walkDirection = 2.0f;  
GLfloat headNeckAngle = 0.0f;   

void drawPolyLine(const std::vector<std::pair<GLfloat, GLfloat>>& polyline) {
    glBegin(GL_LINE_STRIP);
    for (const auto& point : polyline) {
        glVertex2f(point.first, point.second);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
    glTranslatef(dinosaurPosition, 0.0f, 0.0f);

    for (int iPoly = 0; iPoly < polylines.size(); iPoly++) {
        if (iPoly == 2) {  // Front Left leg
            glPushMatrix();
            glTranslatef(110, 150, 0);  // Adjust these values for the pivot point
            glRotatef(legAngle, 0.0f, 0.0f, 1.0f);
            glTranslatef(-110, -150, 0);
        }
        if (iPoly == 5) {  // Back Left leg
            glPushMatrix();
            glTranslatef(50, 100, 0);  // Adjust these values for the pivot point
            glRotatef(legAngle * 0.3, 0.0f, 0.0f, 1.0f);  // Reduced rotation for leg 5
            glTranslatef(-50, -100, 0);
        }
        if (iPoly == 9 || iPoly == 6 || iPoly == 7) {  // Back Right leg and associated parts
            glPushMatrix();
            glTranslatef(350, 100, 0);  // Adjust these values for the pivot point
            glRotatef(-legAngle * 0.3, 0.0f, 0.0f, 1.0f);  // Reduced rotation for leg 9
            glTranslatef(-350, -100, 0);
        } 
         if (iPoly == 10) {  // Front Right leg
            glPushMatrix();
            glTranslatef(300, 150, 0);  // Adjust these values for the pivot point
            glRotatef(-legAngle * 0.3, 0.0f, 0.0f, 1.0f);  // Reduced rotation for leg 10
            glTranslatef(-300, -150, 0);
        } 
        /*if (iPoly == 16) {
         glPushMatrix();
            glTranslatef(150, 250, 0);  // Adjust these values for the pivot point of the head
            glRotatef(headNeckAngle, 0.0f, 0.0f, 1.0f);  // Apply head and neck rotation
            glTranslatef(-150, -250, 0);
        } */
        if (iPoly == 0) {
         glPushMatrix();
            glTranslatef(150, 250, 0);  // Adjust these values for the pivot point of the head 
            glRotatef(headNeckAngle, 0.0f, 0.0f, 1.0f);  // Apply head and neck rotation
            glTranslatef(-150, -250, 0);
            } 
        if (iPoly == 11) {
         glPushMatrix();
            glTranslatef(150, 250, 0);  // Adjust these values for the pivot point of the head
            glRotatef(headNeckAngle, 0.0f, 0.0f, 1.0f);  // Apply head and neck rotation
            glTranslatef(-150, -250, 0);
        }
        if (iPoly == 12) {
         glPushMatrix();
            glTranslatef(150, 250, 0);  // Adjust these values for the pivot point of the head
            glRotatef(headNeckAngle, 0.0f, 0.0f, 1.0f);  // Apply head and neck rotation
            glTranslatef(-150, -250, 0);
        }
        if (iPoly == 13) {
         glPushMatrix();
            glTranslatef(150, 250, 0);  // Adjust these values for the pivot point of the head
            glRotatef(headNeckAngle, 0.0f, 0.0f, 1.0f);  // Apply head and neck rotation
            glTranslatef(-150, -250, 0);
        } 
        if (iPoly == 14) {
         glPushMatrix();
            glTranslatef(150, 250, 0);  // Adjust these values for the pivot point of the head
            glRotatef(headNeckAngle, 0.0f, 0.0f, 1.0f);  // Apply head and neck rotation
            glTranslatef(-150, -250, 0);
        }

        drawPolyLine(polylines[iPoly]);

        if (iPoly == 0 || /* iPoly == 16 ||*/ iPoly == 2 || iPoly == 5 || iPoly == 9 || iPoly == 6 || iPoly == 7 || iPoly == 10 || iPoly == 11 || iPoly == 12 || iPoly == 13 || iPoly == 14) {
            glPopMatrix();
        }
    }

    glPopMatrix();
    glFlush();
}

void animate(int value) {
    dinosaurPosition += walkDirection;

    legAngle += 3.0f * direction;
    if (legAngle > 10.0f || legAngle < -10.0f) {
        direction = -direction;
    }
    
    headNeckAngle += 1.0f * direction;
    if (headNeckAngle > 5.0f || headNeckAngle < -5.0f) {
        direction = -direction;
        }


    // Check boundaries for dinosaur movement
    if (dinosaurPosition > 300.0f || dinosaurPosition < -300.0f) {  // Adjust these values as needed
        walkDirection = -walkDirection;
    }

    glutPostRedisplay();
    glutTimerFunc(100, animate, 0);
}
void loadPolylines(const char* fileName) {
    std::ifstream inStream;
    inStream.open(fileName);
    if (inStream.fail()) return;

    GLint numpolys, numLines;
    GLfloat x, y;
    inStream >> numpolys;

    for (int iPoly = 0; iPoly < numpolys; iPoly++) {
        inStream >> numLines;
        std::vector<std::pair<GLfloat, GLfloat>> polyline;
        for (int i = 0; i < numLines; i++) {
            inStream >> x >> y;
            polyline.push_back({x, y});
        }
        polylines.push_back(polyline);
    }
    inStream.close();
}

int main(int argc, char** argv) {
    loadPolylines("dino.dat");

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Dinosaur Walking");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1024.0, 0, 768.0);

    glutDisplayFunc(display);
    glutTimerFunc(100, animate, 0);
    glutMainLoop();

    return(0);
}

    