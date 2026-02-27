// SpecularLightingDemo.cpp
// Per-fragment Phong specular demo (shininess 2..256 like LearnOpenGL reference)
//
// macOS build (legacy OpenGL):
//   clang++ SpecularLightingDemo.cpp -o specular_demo -framework OpenGL -framework GLUT
//
// Linux build (needs GLEW):
//   g++ SpecularLightingDemo.cpp -o specular_demo -lglut -lGL -lGLU -lGLEW
//
// Windows (freeglut + glew):
//   g++ SpecularLightingDemo.cpp -o specular_demo.exe -lfreeglut -lopengl32 -lglu32 -lglew32

#ifdef __APPLE__
  #define GL_SILENCE_DEPRECATION
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
  #include <GLUT/glut.h>
#else
  #include <GL/glew.h>
  #include <GL/glut.h>
  #include <GL/glu.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

// -----------------------------
// Simple camera
// -----------------------------
struct Camera {
  float ex, ey, ez;
  float cx, cy, cz;
  float ux, uy, uz;
};

static Camera gCamera = { 
  -1.4f, 0.0f, 8.0f,   // eye (moved left)
   0.0f, 0.0f, 0.0f,   // center
   0.0f, 1.0f, 0.0f    // up
};
static int gWinW = 1200, gWinH = 800;

// -----------------------------
// Cube instances
// -----------------------------
struct SpecularCube {
  float shininess;
  float position[3];
  const char* label;
};

static std::vector<SpecularCube> gCubes;

// Material / light (match the “orange blocks” look)
static const float kObjectColor[3] = { 0.98f, 0.53f, 0.30f };  // more copper/orange
static const float kAmbientStrength = 0.04f;  // keep dark sides dark
static const float kDiffuseStrength = 0.80f;  // reduce wash
static const float kSpecStrength    = 1.10f;  // still bright highlights

// Put light up-left-ish so top-left cube is brightest like reference
static float gLightPosWorld[3] = { 3.0f, 4.0f, 6.0f };  // closer = more concentrated highlight

// Rotation like the reference: slight yaw so you see a side face
static float gCubeYawDeg = 22.0f;

// -----------------------------
// Shader helpers
// -----------------------------
static std::string ReadTextFile(const char* path) {
  std::ifstream f(path);
  if (!f) return "";
  std::stringstream ss;
  ss << f.rdbuf();
  return ss.str();
}

static void PrintShaderLog(GLuint shader, const char* name) {
  GLint ok = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
  if (ok) return;

  GLint len = 0;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

  if (len <= 1) {
    std::cerr << "\n[Shader compile failed] " << name << "\n(no log)\n";
    return;
  }

  std::vector<GLchar> buf((size_t)len, 0);
  GLsizei outLen = 0;
  glGetShaderInfoLog(shader, len, &outLen, buf.data());

  std::cerr << "\n[Shader compile failed] " << name << "\n"
            << buf.data() << "\n";
}

static void PrintProgramLog(GLuint prog) {
  GLint ok = 0;
  glGetProgramiv(prog, GL_LINK_STATUS, &ok);
  if (ok) return;

  GLint len = 0;
  glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);

  if (len <= 1) {
    std::cerr << "\n[Program link failed]\n(no log)\n";
    return;
  }

  std::vector<GLchar> buf((size_t)len, 0);
  GLsizei outLen = 0;
  glGetProgramInfoLog(prog, len, &outLen, buf.data());

  std::cerr << "\n[Program link failed]\n"
            << buf.data() << "\n";
}

static GLuint CompileShader(GLenum type, const char* path) {
  std::string src = ReadTextFile(path);
  if (src.empty()) {
    std::cerr << "ERROR: Could not read shader file: " << path << "\n";
    std::exit(1);
  }

  GLuint sh = glCreateShader(type);
  const char* csrc = src.c_str();
  glShaderSource(sh, 1, &csrc, nullptr);
  glCompileShader(sh);

  PrintShaderLog(sh, path);
  return sh;
}

static GLuint gProg = 0;

// Uniform locations
static GLint uLightPosEye   = -1;
static GLint uObjectColor   = -1;
static GLint uAmbientStr    = -1;
static GLint uDiffuseStr    = -1;
static GLint uSpecStr       = -1;
static GLint uShininess     = -1;

// Multiply 4x4 column-major matrix by vec4
static void MulMat4Vec4(const float M[16], const float v[4], float out[4]) {
  for (int r = 0; r < 4; ++r) {
    out[r] = M[0*4 + r]*v[0] + M[1*4 + r]*v[1] + M[2*4 + r]*v[2] + M[3*4 + r]*v[3];
  }
}

// Transform world point into eye space using current view matrix (MODELVIEW with no model)
static void WorldToEyePoint(const float mv[16], const float pWorld[3], float pEyeOut[3]) {
  float v[4] = { pWorld[0], pWorld[1], pWorld[2], 1.0f };
  float o[4];
  MulMat4Vec4(mv, v, o);
  pEyeOut[0] = o[0]; pEyeOut[1] = o[1]; pEyeOut[2] = o[2];
}

// -----------------------------
// Scene setup
// -----------------------------
static void InitCubes() {
  // positions tuned to match the reference spacing
  const float xs[4] = { -3.2f, -1.05f, 1.05f, 3.2f };
  const float yTop =  1.25f;
  const float yBot = -1.25f;
  const float z    =  0.0f;

  gCubes = {
    { 2.0f,   {xs[0], yTop, z}, "2"   },
    { 4.0f,   {xs[1], yTop, z}, "4"   },
    { 8.0f,   {xs[2], yTop, z}, "8"   },
    { 16.0f,  {xs[3], yTop, z}, "16"  },
    { 32.0f,  {xs[0], yBot, z}, "32"  },
    { 64.0f,  {xs[1], yBot, z}, "64"  },
    { 128.0f, {xs[2], yBot, z}, "128" },
    { 256.0f, {xs[3], yBot, z}, "256" }
  };
}

// -----------------------------
// Geometry (immediate mode, but shaded by GLSL)
// -----------------------------
static void DrawCube(float size = 1.5f) {
  const float h = size * 0.5f;

  glBegin(GL_QUADS);

  // +Z (front)
  glNormal3f(0,0,1);
  glVertex3f(-h,-h, h); glVertex3f( h,-h, h); glVertex3f( h, h, h); glVertex3f(-h, h, h);

  // -Z (back)
  glNormal3f(0,0,-1);
  glVertex3f(-h,-h,-h); glVertex3f(-h, h,-h); glVertex3f( h, h,-h); glVertex3f( h,-h,-h);

  // +Y (top)
  glNormal3f(0,1,0);
  glVertex3f(-h, h,-h); glVertex3f(-h, h, h); glVertex3f( h, h, h); glVertex3f( h, h,-h);

  // -Y (bottom)
  glNormal3f(0,-1,0);
  glVertex3f(-h,-h,-h); glVertex3f( h,-h,-h); glVertex3f( h,-h, h); glVertex3f(-h,-h, h);

  // +X (right)
  glNormal3f(1,0,0);
  glVertex3f( h,-h,-h); glVertex3f( h, h,-h); glVertex3f( h, h, h); glVertex3f( h,-h, h);

  // -X (left)
  glNormal3f(-1,0,0);
  glVertex3f(-h,-h,-h); glVertex3f(-h,-h, h); glVertex3f(-h, h, h); glVertex3f(-h, h,-h);

  glEnd();
}

// -----------------------------
// Labels: project cube center to screen & draw bitmap text under it
// -----------------------------
static void DrawBitmapTextScreen(const char* s, int x, int y) {
  glRasterPos2i(x, y);
  while (*s) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *s++);
}

static void DrawLabels() {
  // Grab current matrices / viewport (after rendering cubes)
  double model[16], proj[16];
  int viewport[4];
  glGetDoublev(GL_MODELVIEW_MATRIX, model);
  glGetDoublev(GL_PROJECTION_MATRIX, proj);
  glGetIntegerv(GL_VIEWPORT, viewport);

  // Switch to 2D screen space
  glUseProgram(0);
  glDisable(GL_DEPTH_TEST);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, gWinW, 0, gWinH);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glColor3f(1,1,1);

  for (const auto& c : gCubes) {
    // Project cube center (world) — close enough for label placement
    double sx, sy, sz;
    gluProject(c.position[0], c.position[1], c.position[2],
               model, proj, viewport, &sx, &sy, &sz);

    // Put label a bit below the projected cube center
    int px = (int)sx - 10;          // small centering tweak
    int py = (int)sy - 140;         // tuned for cube size + camera
    if (py < 10) py = 10;

    DrawBitmapTextScreen(c.label, px, py);
  }

  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);

  glEnable(GL_DEPTH_TEST);
}

// -----------------------------
// GLUT callbacks
// -----------------------------
static void Display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Camera / view
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(gCamera.ex, gCamera.ey, gCamera.ez,
            gCamera.cx, gCamera.cy, gCamera.cz,
            gCamera.ux, gCamera.uy, gCamera.uz);

  // Compute light position in EYE space (view * lightWorld)
  float mvView[16];
  glGetFloatv(GL_MODELVIEW_MATRIX, mvView);
  float lightEye[3];
  WorldToEyePoint(mvView, gLightPosWorld, lightEye);

  glUseProgram(gProg);

  // Global uniforms
  glUniform3f(uLightPosEye, lightEye[0], lightEye[1], lightEye[2]);
  glUniform3f(uObjectColor, kObjectColor[0], kObjectColor[1], kObjectColor[2]);
  glUniform1f(uAmbientStr, kAmbientStrength);
  glUniform1f(uDiffuseStr, kDiffuseStrength);
  glUniform1f(uSpecStr,    kSpecStrength);

  // Draw cubes
  for (const auto& cube : gCubes) {
    glPushMatrix();
      glTranslatef(cube.position[0], cube.position[1], cube.position[2]);
      glRotatef(gCubeYawDeg, 0, 1, 0);
      glUniform1f(uShininess, cube.shininess);
      DrawCube(1.5f);
    glPopMatrix();
  }

  // Labels (2D overlay)
  DrawLabels();

  glutSwapBuffers();
}

static void Reshape(int w, int h) {
  if (h <= 0) h = 1;
  gWinW = w; gWinH = h;

  const float aspect = (float)w / (float)h;
  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, aspect, 0.1, 100.0);

  glMatrixMode(GL_MODELVIEW);
}

static void Keyboard(unsigned char key, int, int) {
  if (key == 27) std::exit(0);

  // Optional: tiny light nudges if you want to tweak visually
  if (key == 'j') gLightPosWorld[0] -= 0.2f;
  if (key == 'l') gLightPosWorld[0] += 0.2f;
  if (key == 'i') gLightPosWorld[1] += 0.2f;
  if (key == 'k') gLightPosWorld[1] -= 0.2f;

  glutPostRedisplay();
}

// -----------------------------
// Init
// -----------------------------
static void InitGL() {
#ifndef __APPLE__
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    std::cerr << "GLEW init failed: " << glewGetErrorString(err) << "\n";
    std::exit(1);
  }
#endif

  glEnable(GL_DEPTH_TEST);
  glClearColor(0.07f, 0.07f, 0.07f, 1.0f); // dark background like reference

  // Build shader program
  GLuint vs = CompileShader(GL_VERTEX_SHADER,   "phong.vert");
  GLuint fs = CompileShader(GL_FRAGMENT_SHADER, "phong.frag");

  gProg = glCreateProgram();
  glAttachShader(gProg, vs);
  glAttachShader(gProg, fs);
  glLinkProgram(gProg);
  PrintProgramLog(gProg);

  glDeleteShader(vs);
  glDeleteShader(fs);

  // Uniform locations
  glUseProgram(gProg);
  uLightPosEye = glGetUniformLocation(gProg, "uLightPosEye");
  uObjectColor = glGetUniformLocation(gProg, "uObjectColor");
  uAmbientStr  = glGetUniformLocation(gProg, "uAmbientStrength");
  uDiffuseStr  = glGetUniformLocation(gProg, "uDiffuseStrength");
  uSpecStr     = glGetUniformLocation(gProg, "uSpecularStrength");
  uShininess   = glGetUniformLocation(gProg, "uShininess");

  if (uLightPosEye < 0 || uObjectColor < 0 || uShininess < 0) {
    std::cerr << "ERROR: Missing uniforms. Check shader files are correct.\n";
    std::exit(1);
  }

  glUseProgram(0);
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(gWinW, gWinH);
  glutCreateWindow("Specular Lighting Demo - Different Shininess Values");

  InitGL();
  InitCubes();

  glutDisplayFunc(Display);
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Keyboard);

  std::cout << "Controls: ESC to quit. (Optional) I/K/J/L nudge light.\n";
  glutMainLoop();
  return 0;
}