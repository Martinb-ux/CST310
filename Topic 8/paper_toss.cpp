#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>
#include <cstdio>
#include <cstring>

float t = 0.0f;
float aim = 0.0f, power = 1.0f, wind = 0.0f;
float px = 0, py = 0.22f, pz = 2.4f;
float vx = 0, vy = 0, vz = 0;
float binX = 0.0f, binZ = -5.5f, binR = 0.65f, binH = 1.3f;
bool flying = false, scored = false, bounced = false;
int score = 0, shots = 0;

void drawText(float x, float y, const char* s){
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D(0,900,0,650);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    glDisable(GL_DEPTH_TEST); glColor3f(1,1,1); glRasterPos2f(x,y);
    while(*s) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*s++);
    glEnable(GL_DEPTH_TEST);
    glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix(); glMatrixMode(GL_MODELVIEW);
}

void resetBall(){
    px = aim * 0.25f; py = 0.22f; pz = 2.4f;
    vx = vy = vz = 0.0f;
    flying = scored = bounced = false;
}

void drawRoom(){
    glBegin(GL_QUADS);
    glColor3f(0.55f,0.55f,0.60f); // floor
    glVertex3f(-8,0,-12); glVertex3f(8,0,-12); glVertex3f(8,0,4); glVertex3f(-8,0,4);

    glColor3f(0.70f,0.80f,0.95f); // back wall
    glVertex3f(-8,0,-12); glVertex3f(8,0,-12); glVertex3f(8,5,-12); glVertex3f(-8,5,-12);

    glColor3f(0.85f,0.85f,0.88f); // left wall
    glVertex3f(-8,0,-12); glVertex3f(-8,0,4); glVertex3f(-8,5,4); glVertex3f(-8,5,-12);

    glColor3f(0.85f,0.85f,0.88f); // right wall
    glVertex3f(8,0,-12); glVertex3f(8,0,4); glVertex3f(8,5,4); glVertex3f(8,5,-12);
    glEnd();

    // floor stripes
    glColor3f(0.45f,0.45f,0.50f);
    for(float z=-12; z<=4; z+=1.0f){
        glBegin(GL_LINES);
        glVertex3f(-8,0.001f,z); glVertex3f(8,0.001f,z);
        glEnd();
    }
}

void drawDesk(){
    glPushMatrix();
    glTranslatef(0,0,1.8f);
    glColor3f(0.45f,0.28f,0.16f);
    glBegin(GL_QUADS);
    glVertex3f(-1.8f,0.55f,-0.8f); glVertex3f(1.8f,0.55f,-0.8f); glVertex3f(1.8f,0.55f,0.8f); glVertex3f(-1.8f,0.55f,0.8f);
    glEnd();
    for(int i=0;i<4;i++){
        float x = (i<2)?-1.5f:1.5f, z = (i%2==0)?-0.5f:0.5f;
        glPushMatrix();
        glTranslatef(x,0.27f,z);
        glScalef(0.12f,0.55f,0.12f);
        glutSolidCube(1.0);
        glPopMatrix();
    }
    glPopMatrix();
}

void drawBin(){
    glPushMatrix();
    glTranslatef(binX,0,binZ);

    // shadow
    glColor3f(0.1f,0.1f,0.1f);
    glBegin(GL_POLYGON);
    for(int i=0;i<32;i++){
        float a=2*3.14159f*i/32;
        glVertex3f(0.75f*cos(a),0.001f,0.75f*sin(a));
    }
    glEnd();

    // body
    glColor3f(0.15f,0.15f,0.18f);
    glBegin(GL_QUAD_STRIP);
    for(int i=0;i<=32;i++){
        float a=2*3.14159f*i/32, r1=binR, r2=0.48f;
        glVertex3f(r1*cos(a),0,r1*sin(a));
        glVertex3f(r2*cos(a),binH,r2*sin(a));
    }
    glEnd();

    // rim
    glColor3f(0.78f,0.78f,0.82f);
    glBegin(GL_QUAD_STRIP);
    for(int i=0;i<=32;i++){
        float a=2*3.14159f*i/32, r=0.50f;
        glVertex3f(r*cos(a),binH,r*sin(a));
        glVertex3f((r+0.08f)*cos(a),binH+0.05f,(r+0.08f)*sin(a));
    }
    glEnd();

    glPopMatrix();
}

void drawPaper(){
    glPushMatrix();
    glTranslatef(px,py,pz);
    glRotatef(400*t,1,0.7f,0.2f);
    glColor3f(0.98f,0.98f,0.93f);
    glutSolidSphere(0.14,18,18);
    glColor3f(0.88f,0.88f,0.84f);
    glutWireSphere(0.141,10,10);
    glPopMatrix();
}

void launch(){
    resetBall();
    flying = true;
    shots++;
    vx = aim * 0.09f + wind * 0.15f;
    vy = 0.14f + power * 0.05f;
    vz = -(0.18f + power * 0.12f);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0,3.0,6.5, 0,1.2,-5.5, 0,1,0);

    drawRoom();
    drawDesk();
    drawBin();
    drawPaper();

    if(!flying){
        glColor3f(1.0f,0.95f,0.2f);
        glBegin(GL_LINES);
        glVertex3f(px,py,pz);
        glVertex3f(px + aim*2.0f + wind*1.5f, py + power*0.8f + 0.8f, pz - 3.0f - power*2.0f);
        glEnd();
    }

    char buf[128];
    std::sprintf(buf,"Score: %d   Shots: %d   Aim: %.1f   Power: %.1f   Wind: %.2f",score,shots,aim,power,wind);
    drawText(18,620,buf);
    drawText(18,592,"Left/Right = Aim   Up/Down = Power   Space = Throw   R = Reset Ball   C = Reset Score");

    if(scored) drawText(380,620,"Made it!");
    else if(!flying && shots>0) drawText(360,620,"Ready for next shot");

    glutSwapBuffers();
}

void idle(){
    t += 0.016f;
    wind = 0.35f * std::sin(t * 0.8f + 1.0f);
    binX = 1.4f * std::sin(t * 0.9f);

    if(flying){
        px += vx;
        py += vy;
        pz += vz;
        vx += wind * 0.002f;
        vy -= 0.0052f;

        float dx = px - binX, dz = pz - binZ;
        float d = std::sqrt(dx*dx + dz*dz);

        // score if ball drops inside the bin opening
        if(!scored && py < binH && py > 0.45f && d < 0.42f && vy < 0){
            scored = true;
            score++;
        }

        // rim collision
        if(py > binH-0.03f && py < binH+0.12f && d > 0.42f && d < 0.62f){
            float nx = dx / (d + 0.0001f), nz = dz / (d + 0.0001f);
            float dot = vx*nx + vz*nz;
            vx -= 1.7f * dot * nx;
            vz -= 1.7f * dot * nz;
            vy *= 0.75f;
            px += nx * 0.05f;
            pz += nz * 0.05f;
        }

        // back/side wall bounce if missed hard
        if(pz < -11.8f){ pz = -11.8f; vz *= -0.45f; }
        if(px < -7.8f){ px = -7.8f; vx *= -0.45f; }
        if(px >  7.8f){ px =  7.8f; vx *= -0.45f; }

        // floor bounce
        if(py < 0.14f){
            py = 0.14f;
            if(!bounced){
                vy *= -0.35f;
                vx *= 0.75f;
                vz *= 0.75f;
                bounced = true;
            } else {
                flying = false;
                vy = 0;
            }
        }

        if(std::fabs(vx)+std::fabs(vy)+std::fabs(vz) < 0.01f && py <= 0.14f) flying = false;
    }

    glutPostRedisplay();
}

void key(unsigned char k,int,int){
    if(k==' ') { if(!flying) launch(); }
    if(k=='r') resetBall();
    if(k=='c'){ score=0; shots=0; resetBall(); }
}

void special(int k,int,int){
    if(flying) return;
    if(k==GLUT_KEY_LEFT)  aim -= 0.15f;
    if(k==GLUT_KEY_RIGHT) aim += 0.15f;
    if(k==GLUT_KEY_UP)    power += 0.08f;
    if(k==GLUT_KEY_DOWN)  power -= 0.08f;
    if(aim < -2.2f) aim = -2.2f;
    if(aim >  2.2f) aim =  2.2f;
    if(power < 0.4f) power = 0.4f;
    if(power > 1.8f) power = 1.8f;
    resetBall();
}

void reshape(int w,int h){
    if(h==0) h=1;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0,(float)w/h,0.1,60.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(900,650);
    glutCreateWindow("3D Paper Toss");
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.58f,0.74f,0.95f,1.0f);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(key);
    glutSpecialFunc(special);
    glutIdleFunc(idle);
    resetBall();
    glutMainLoop();
    return 0;
}