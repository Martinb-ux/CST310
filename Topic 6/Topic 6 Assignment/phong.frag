#version 120

varying vec3 vPosEye;
varying vec3 vNormalEye;

uniform vec3  uLightPosEye;
uniform vec3  uObjectColor;        // treated as sRGB input
uniform float uAmbientStrength;
uniform float uDiffuseStrength;
uniform float uSpecularStrength;
uniform float uShininess;

void main() {
  vec3 N = normalize(vNormalEye);
  vec3 Lvec = uLightPosEye - vPosEye;
  vec3 L = normalize(Lvec);
  vec3 V = normalize(-vPosEye);

  // --- sRGB -> linear (important!)
  vec3 albedo_srgb = uObjectColor;
  vec3 albedo = pow(albedo_srgb, vec3(2.2));

  vec3 lightColor_srgb = vec3(1.0, 0.95, 0.85);   // warm light
  vec3 lightColor = pow(lightColor_srgb, vec3(2.2));

  // Ambient (linear)
  vec3 ambient = uAmbientStrength * albedo;

  // Diffuse (linear)
  float diff = max(dot(N, L), 0.0);
  vec3 diffuse = (uDiffuseStrength * diff) * (albedo * lightColor);

  // Specular (Phong, linear)
  // Specular (Blinn-Phong)
  vec3 H = normalize(L + V);
  float spec = 0.0;
  if (diff > 0.0) {
    // Blinn is "tighter" than Phong for same exponent, so scale shininess
    spec = pow(max(dot(N, H), 0.0), uShininess * 4.0);
  }
  vec3 specular = (uSpecularStrength * spec) * lightColor;

  vec3 colorLinear = ambient + diffuse + specular;

  // --- linear -> sRGB (use 2.2, not 2.0)
  vec3 colorSRGB = pow(colorLinear, vec3(1.0 / 2.2));

  gl_FragColor = vec4(colorSRGB, 1.0);
}