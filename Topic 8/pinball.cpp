#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>
#include <cstdio>

float t=0, bx=0, by=0.22f, bz=3.8f, vx=0, vz=0;
bool launched=false;
int score=0;

float lFlip=20, rFlip=-20;
bool lHeld=false, rHeld=false;

struct Bumper{ float x,z,r; };
Bumper bumpers[3]={{-1.2f,-2.0f,0.35f},{1.2f,-2.2f,0.35f},{0.0f,-3.2f,0.4f}};

void text(float x,float y,const char* s){
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D(0,900,0,700);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    glDisable(GL_DEPTH_TEST); glRasterPos2f(x,y);
    while(*s) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*s++);
    glEnable(GL_DEPTH_TEST);
    glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix(); glMatrixMode(GL_MODELVIEW);
}

void resetBall(){
    bx=0; by=0.22f; bz=3.8f; vx=0; vz=0; launched=false;
}

void drawTable(){
    glColor3f(0.15f,0.15f,0.18f);
    glBegin(GL_QUADS);
    glVertex3f(-3,0,-7); glVertex3f(3,0,-7); glVertex3f(3,0,4.5f); glVertex3f(-3,0,4.5f);
    glEnd();

    glColor3f(0.7f,0.1f,0.1f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-2.7f,0.01f,4.2f); glVertex3f(-2.7f,0.01f,-6.6f);
    glVertex3f(2.7f,0.01f,-6.6f); glVertex3f(2.7f,0.01f,4.2f);
    glEnd();

    glColor3f(0.3f,0.3f,0.35f);
    glBegin(GL_QUADS);
    glVertex3f(-0.7f,0.01f,4.2f); glVertex3f(0.7f,0.01f,4.2f);
    glVertex3f(0.35f,0.01f,3.2f); glVertex3f(-0.35f,0.01f,3.2f);
    glEnd();
}

void drawBumpers(){
    for(int i=0;i<3;i++){
        glPushMatrix();
        glTranslatef(bumpers[i].x,0.22f,bumpers[i].z);
        glColor3f(0.2f,0.7f,1.0f);
        glutSolidSphere(bumpers[i].r,18,18);
        glPopMatrix();
    }
}

void drawBall(){
    glPushMatrix();
    glTranslatef(bx,by,bz);
    glColor3f(1,1,0.9f);
    glutSolidSphere(0.16,18,18);
    glPopMatrix();
}

void flipper(float x,float z,float ang){
    glPushMatrix();
    glTranslatef(x,0.12f,z);
    glRotatef(ang,0,1,0);
    glTranslatef(0.55f,0,0);
    glScalef(1.1f,0.2f,0.28f);
    glutSolidCube(1);
    glPopMatrix();
}

void drawFlippers(){
    glColor3f(1.0f,0.8f,0.1f);
    flipper(-0.75f,3.1f,lFlip);
    flipper( 0.75f,3.1f,180+rFlip);
}

void launchBall(){
    if(!launched){
        launched=true;
        vx=0.0f;
        vz=-0.16f;
    }
}

void bumperHit(float cx,float cz,float r){
    float dx=bx-cx, dz=bz-cz, d=sqrt(dx*dx+dz*dz);
    if(d < r+0.16f){
        float nx=dx/(d+0.0001f), nz=dz/(d+0.0001f);
        float dot=vx*nx+vz*nz;
        vx -= 2.0f*dot*nx;
        vz -= 2.0f*dot*nz;
        vx += 0.02f*nx;
        vz += 0.02f*nz;
        bx = cx + (r+0.17f)*nx;
        bz = cz + (r+0.17f)*nz;
        score += 10;
    }
}

void flipperHit(float px,float pz,float angDeg){
    float a=angDeg*3.14159f/180.0f;
    float cx=px+0.55f*cos(a), cz=pz-0.55f*sin(a);
    float dx=bx-cx, dz=bz-cz, d=sqrt(dx*dx+dz*dz);
    if(d<0.55f){
        vx += 0.03f*cos(a);
        vz -= 0.03f*sin(a) + 0.02f;
    }
}

void display(){
    char buf[64];
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0,5.8,7.2, 0,0,-1.5, 0,1,0);

    glRotatef(18,1,0,0);
    drawTable();
    drawBumpers();
    drawFlippers();
    drawBall();

    std::sprintf(buf,"Score: %d",score);
    glColor3f(1,1,1);
    text(20,670,buf);
    text(20,645,"A/D = flippers   Space = launch   R = reset");

    glutSwapBuffers();
}

void idle(){
    t += 0.016f;

    if(lHeld && lFlip<65) lFlip += 4.0f;
    if(!lHeld && lFlip>20) lFlip -= 4.0f;
    if(rHeld && rFlip>-65) rFlip -= 4.0f;
    if(!rHeld && rFlip<-20) rFlip += 4.0f;

    if(launched){
        bx += vx;
        bz += vz;
        vz += 0.0008f; // slight pull downward toward player
        vx *= 0.995f; vz *= 0.995f;

        if(bx<-2.55f){ bx=-2.55f; vx*=-1; }
        if(bx> 2.55f){ bx= 2.55f; vx*=-1; }
        if(bz<-6.35f){ bz=-6.35f; vz*=-1; }

        bumperHit(bumpers[0].x,bumpers[0].z,bumpers[0].r);
        bumperHit(bumpers[1].x,bumpers[1].z,bumpers[1].r);
        bumperHit(bumpers[2].x,bumpers[2].z,bumpers[2].r);

        flipperHit(-0.75f,3.1f,lFlip);
        flipperHit(0.75f,3.1f,180+rFlip);

        if(bz>4.35f) resetBall();
    }

    glutPostRedisplay();
}

void key(unsigned char k,int,int){
    if(k=='a'||k=='A') lHeld=true;
    if(k=='d'||k=='D') rHeld=true;
    if(k==' ') launchBall();
    if(k=='r'||k=='R'){ score=0; resetBall(); }
}

void keyUp(unsigned char k,int,int){
    if(k=='a'||k=='A') lHeld=false;
    if(k=='d'||k=='D') rHeld=false;
}

void reshape(int w,int h){
    if(h==0) h=1;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,(float)w/h,0.1,50);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(900,700);
    glutCreateWindow("3D Pinball");
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.05f,0.05f,0.08f,1);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutKeyboardUpFunc(keyUp);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}