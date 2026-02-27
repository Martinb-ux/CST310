// phong.vert (GLSL 1.20 compatible)
#version 120

varying vec3 vPosEye;
varying vec3 vNormalEye;

void main() {
  vec4 posEye = gl_ModelViewMatrix * gl_Vertex;
  vPosEye = posEye.xyz;

  vNormalEye = normalize(gl_NormalMatrix * gl_Normal);

  gl_Position = gl_ProjectionMatrix * posEye;
}