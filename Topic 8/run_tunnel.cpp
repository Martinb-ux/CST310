#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>

float t = 0.0f;

void cube(float x,float y,float z,float s){
    glPushMatrix();
    glTranslatef(x,y,z);
    glutSolidCube(s);
    glPopMatrix();
}

void drawTunnel(){
    const int rings = 80, seg = 32;
    const float dz = 0.6f;

    for(int i=0;i<rings;i++){
        float z1 = -i * dz;
        float z2 = -(i + 1) * dz;

        glBegin(GL_QUAD_STRIP);
        for(int j=0;j<=seg;j++){
            float th = 2.0f * 3.14159f * j / seg;

            float r1 = 2.5f + 0.25f * sinf(3*th + 0.35f*z1 + 2*t);
            float r2 = 2.5f + 0.25f * sinf(3*th + 0.35f*z2 + 2*t);

            float pulse1 = 0.5f + 0.5f * sinf(th + z1 + t);
            float pulse2 = 0.5f + 0.5f * sinf(th + z2 + t);

            glColor3f(0.2f + 0.6f*pulse1, 0.3f, 0.8f);
            glVertex3f(r1*cosf(th), r1*sinf(th), z1);

            glColor3f(0.7f, 0.2f + 0.6f*pulse2, 0.9f);
            glVertex3f(r2*cosf(th), r2*sinf(th), z2);
        }
        glEnd();
    }
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0,0,0.8, 0,0,-8, 0,1,0);

    glRotatef(18.0f * sinf(t * 0.8f), 0, 0, 1);
    drawTunnel();

    float runnerA = 1.2f * sinf(t);
    float rr = 2.1f;
    glColor3f(1,1,1);
    cube(rr*cosf(runnerA), rr*sinf(runnerA), -4.0f, 0.22f);

    for(int k=0;k<8;k++){
        float z = -10.0f - fmod(t*6.0f + k*5.0f, 40.0f);
        float a = 1.4f + 0.8f * sinf(k + t*1.2f);
        glColor3f(1,0.3f,0.2f);
        cube(2.05f*cosf(a), 2.05f*sinf(a), z, 0.3f);
    }

    glutSwapBuffers();
}

void idle(){
    t += 0.016f;
    glutPostRedisplay();
}

void reshape(int w,int h){
    if(h==0) h=1;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75.0, (float)w/h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000,700);
    glutCreateWindow("Run Inspired Tunnel");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.02f,0.02f,0.08f,1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}