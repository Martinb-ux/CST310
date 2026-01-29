// DoorOnly_FreeCamera.cpp (FreeGLUT + legacy OpenGL fixed-function)

#include <cmath>
#include <algorithm>
#include <iostream>

#ifdef _WIN32
  #include <windows.h>
#endif

#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

// ---------------------------
// Camera state
// ---------------------------
float gCamX = 0.0f, gCamY = 1.5f, gCamZ = 6.0f;
float gYawDeg = 180.0f;   // looking toward -Z-ish (adjust as you like)
float gPitchDeg = 0.0f;

bool gKeys[256] = { false };
bool gSpecial[256] = { false };

int gPrevTimeMs = 0;

// ---------------------------
// Helpers
// ---------------------------
static float clampf(float v, float lo, float hi) {
  return std::max(lo, std::min(v, hi));
}

static float deg2rad(float d) { return d * 3.1415926535f / 180.0f; }

void SetMaterial(float r, float g, float b, float shininess = 16.0f,
                 float spec = 0.10f) {
  float diffuse[]  = { r, g, b, 1.0f };
  float ambient[]  = { r * 0.35f, g * 0.35f, b * 0.35f, 1.0f };
  float specular[] = { spec, spec, spec, 1.0f };

  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

void DrawUnitCube() {
  glutSolidCube(1.0);
}

void DrawBox(float sx, float sy, float sz) {
  glPushMatrix();
  glScalef(sx, sy, sz);
  DrawUnitCube();
  glPopMatrix();
}

// ---------------------------
// Lighting
// ---------------------------
void SetupLights() {
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  float lightPos[]     = { 2.0f, 4.0f, 3.0f, 1.0f };
  float lightDiffuse[] = { 0.70f, 0.70f, 0.70f, 1.0f };
  float lightAmbient[] = { 0.15f, 0.15f, 0.15f, 1.0f };

  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiffuse);
  glLightfv(GL_LIGHT0, GL_AMBIENT,  lightAmbient);
}

// ---------------------------
// Door
// ---------------------------
void DrawDoorOnly() {
  // Door dimensions (tweak as needed)
  const float doorW = 1.5f;
  const float doorH = 3.0f;
  const float doorT = 0.06f;

  // Door position (bottom at y=0)
  const float doorX = 0.3f;
  const float doorY = 0.5f;
  const float doorZ = -2.0f;

  // ---------------------------
  // Door panel (dark grey metal)
  // ---------------------------
  SetMaterial(0.35f, 0.35f, 0.36f, 22.0f, 0.06f);

  glPushMatrix();
  glTranslatef(doorX, doorY + doorH * 0.5f, doorZ);
  glScalef(doorW, doorH, doorT);
  DrawUnitCube();
  glPopMatrix();

  // ---------------------------
  // Frame (a little darker grey)
  // ---------------------------
  const float frameW = 0.10f;
  const float frameT = 0.10f;
  SetMaterial(0.22f, 0.22f, 0.23f, 10.0f, 0.02f);

  // Left vertical
  glPushMatrix();
  glTranslatef(doorX - (doorW * 0.5f + frameW * 0.5f), doorY + doorH * 0.5f, doorZ);
  DrawBox(frameW, doorH + frameW, frameT);
  glPopMatrix();

  // Right vertical
  glPushMatrix();
  glTranslatef(doorX + (doorW * 0.5f + frameW * 0.5f), doorY + doorH * 0.5f, doorZ);
  DrawBox(frameW, doorH + frameW, frameT);
  glPopMatrix();

  // Top
  glPushMatrix();
  glTranslatef(doorX, doorY + doorH + frameW * 0.5f, doorZ);
  DrawBox(doorW + frameW * 2.0f, frameW, frameT);
  glPopMatrix();

  // ---------------------------
  // Window (upper-left) - fake inset + glass
  // ---------------------------
  // Window size and placement (relative to door)
  const float winW = 0.22f;
  const float winH = 0.95f;
  const float winInset = 0.015f; // how far "in" it looks
  const float winX = doorX - doorW * 0.5f + 0.35f;  // left side
  const float winY = doorY + doorH * 0.75f;         // moved higher up
  const float winZ = doorZ + doorT * 0.55f;         // near door surface

  // Window frame (dark)
  SetMaterial(0.12f, 0.12f, 0.13f, 14.0f, 0.02f);
  glPushMatrix();
  glTranslatef(winX, winY, winZ);
  DrawBox(winW + 0.06f, winH + 0.06f, 0.03f);
  glPopMatrix();

  // Window "opening" (slightly recessed to suggest a cutout)
  SetMaterial(0.18f, 0.18f, 0.19f, 8.0f, 0.00f);
  glPushMatrix();
  glTranslatef(winX, winY, winZ - winInset);
  DrawBox(winW + 0.01f, winH + 0.01f, 0.02f);
  glPopMatrix();

  // Glass (light bluish/gray, slightly forward)
  SetMaterial(0.25f, 0.30f, 0.35f, 80.0f, 0.25f);
  glPushMatrix();
  glTranslatef(winX, winY, winZ + 0.02f);
  DrawBox(winW, winH, 0.01f);
  glPopMatrix();

  // ---------------------------
  // Handle (left base, knob to right) - lever style
  // ---------------------------
  // Base plate
  SetMaterial(0.55f, 0.55f, 0.55f, 60.0f, 0.20f);
  const float handleBaseX = doorX - doorW * 0.5f + 0.18f;
  const float handleBaseY = doorY + doorH * 0.50f;
  const float handleZ = doorZ + doorT * 0.65f;

  glPushMatrix();
  glTranslatef(handleBaseX, handleBaseY, handleZ);
  DrawBox(0.10f, 0.18f, 0.03f);
  glPopMatrix();

  // Lever (sticks out to the right)
  glPushMatrix();
  glTranslatef(handleBaseX + 0.09f, handleBaseY, handleZ + 0.04f);
  DrawBox(0.22f, 0.04f, 0.08f);
  glPopMatrix();

  // ---------------------------
  // Hinges (3) on right side
  // ---------------------------
  SetMaterial(0.18f, 0.18f, 0.18f, 20.0f, 0.04f);

  const float hingeX = doorX + doorW * 0.5f - 0.03f;
  const float hingeZ = doorZ + doorT * 0.55f;
  const float hingeW = 0.06f, hingeH = 0.18f, hingeT = 0.10f;

  float hingeYs[3] = {
    doorY + doorH * 0.20f,
    doorY + doorH * 0.50f,
    doorY + doorH * 0.80f
  };

  for (int i = 0; i < 3; i++) {
    glPushMatrix();
    glTranslatef(hingeX, hingeYs[i], hingeZ);
    DrawBox(hingeW, hingeH, hingeT);
    glPopMatrix();
  }

  // ---------------------------
  // Door closer (top bar)
  // ---------------------------
  SetMaterial(0.65f, 0.65f, 0.65f, 50.0f, 0.18f);

  // Main closer box near the top
  glPushMatrix();
  glTranslatef(doorX + 0.3f, doorY + doorH - 0.10f, doorZ + doorT * 0.65f);
  DrawBox(0.35f, 0.08f, 0.10f);
  glPopMatrix();

  // Arm (thin bar)
  glPushMatrix();
  glTranslatef(doorX + 0.07f, doorY + doorH - 0.10f, doorZ + doorT * 0.70f);
  DrawBox(0.55f, 0.03f, 0.05f);
  glPopMatrix();
}

// ---------------------------
// Camera update
// ---------------------------
void UpdateCamera(float dt) {
  // Look controls (arrow keys)
  const float lookSpeed = 90.0f; // deg/sec
  if (gSpecial[GLUT_KEY_LEFT])  gYawDeg   -= lookSpeed * dt;
  if (gSpecial[GLUT_KEY_RIGHT]) gYawDeg   += lookSpeed * dt;
  if (gSpecial[GLUT_KEY_UP])    gPitchDeg += lookSpeed * dt;
  if (gSpecial[GLUT_KEY_DOWN])  gPitchDeg -= lookSpeed * dt;

  gPitchDeg = clampf(gPitchDeg, -89.0f, 89.0f);

  // Movement controls
  const float moveSpeed = 3.0f; // units/sec

  float yaw = deg2rad(gYawDeg);
  float pitch = deg2rad(gPitchDeg);

  // Forward direction from yaw/pitch
  float fx = std::cos(pitch) * std::sin(yaw);
  float fy = std::sin(pitch);
  float fz = std::cos(pitch) * std::cos(yaw);

  // Right vector (yaw only keeps strafing level)
  float rx = std::sin(yaw - 3.1415926535f / 2.0f);
  float rz = std::cos(yaw - 3.1415926535f / 2.0f);

  if (gKeys['w'] || gKeys['W']) { gCamX += fx * moveSpeed * dt; gCamY += fy * moveSpeed * dt; gCamZ += fz * moveSpeed * dt; }
  if (gKeys['s'] || gKeys['S']) { gCamX -= fx * moveSpeed * dt; gCamY -= fy * moveSpeed * dt; gCamZ -= fz * moveSpeed * dt; }
  if (gKeys['a'] || gKeys['A']) { gCamX += rx * moveSpeed * dt;                gCamZ += rz * moveSpeed * dt; }
  if (gKeys['d'] || gKeys['D']) { gCamX -= rx * moveSpeed * dt;                gCamZ -= rz * moveSpeed * dt; }

  if (gKeys['q'] || gKeys['Q']) { gCamY -= moveSpeed * dt; }
  if (gKeys['e'] || gKeys['E']) { gCamY += moveSpeed * dt; }
}

// ---------------------------
// GLUT callbacks
// ---------------------------
void Display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Camera look-at
  float yaw = deg2rad(gYawDeg);
  float pitch = deg2rad(gPitchDeg);

  float fx = std::cos(pitch) * std::sin(yaw);
  float fy = std::sin(pitch);
  float fz = std::cos(pitch) * std::cos(yaw);

  gluLookAt(
    gCamX, gCamY, gCamZ,
    gCamX + fx, gCamY + fy, gCamZ + fz,
    0.0, 1.0, 0.0
  );

  SetupLights();
  DrawDoorOnly();

  glutSwapBuffers();
}

void Reshape(int w, int h) {
  if (h <= 0) h = 1;
  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (double)w / (double)h, 0.1, 100.0);
}

void KeyboardDown(unsigned char key, int, int) {
  gKeys[key] = true;
  if (key == 27) std::exit(0); // ESC
}

void KeyboardUp(unsigned char key, int, int) {
  gKeys[key] = false;
}

void SpecialDown(int key, int, int) {
  gSpecial[key] = true;
}

void SpecialUp(int key, int, int) {
  gSpecial[key] = false;
}

void Idle() {
  int now = glutGet(GLUT_ELAPSED_TIME);
  float dt = (now - gPrevTimeMs) / 1000.0f;
  gPrevTimeMs = now;

  UpdateCamera(dt);
  glutPostRedisplay();
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(960, 540);
  glutCreateWindow("Project 4: Door Only + Free Camera");

  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_NORMALIZE);
  glClearColor(0.10f, 0.12f, 0.16f, 1.0f);

  gPrevTimeMs = glutGet(GLUT_ELAPSED_TIME);

  glutDisplayFunc(Display);
  glutReshapeFunc(Reshape);

  glutKeyboardFunc(KeyboardDown);
  glutKeyboardUpFunc(KeyboardUp);

  glutSpecialFunc(SpecialDown);
  glutSpecialUpFunc(SpecialUp);

  glutIdleFunc(Idle);

  glutMainLoop();
  return 0;
}