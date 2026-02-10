/**
 * Fragment Shader - Realistic Industrial Stairwell
 *
 * Features:
 * - Harsh fluorescent overhead lighting
 * - Very low ambient (dark shadows)
 * - Strong ambient occlusion at corners/floor
 * - Per-material specular (metal vs matte)
 * - Desaturated institutional color grading
 */

#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 Color;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float ambientStrength;

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);

    // ============================================================
    // MATERIAL DETECTION FROM COLOR
    // ============================================================
    float brightness = (Color.r + Color.g + Color.b) / 3.0;
    float saturation = max(max(Color.r, Color.g), Color.b) - min(min(Color.r, Color.g), Color.b);

    // Detect material type
    float roughness = 0.85;      // Default: rough brick/concrete
    float specularStr = 0.08;
    float metallic = 0.0;

    // Gray tones = metal (door, frame, hardware)
    if (saturation < 0.15 && brightness > 0.25 && brightness < 0.65) {
        roughness = 0.45;
        specularStr = 0.4;
        metallic = 0.5;
    }

    // Very bright = light fixture (emissive-ish)
    if (brightness > 0.75) {
        roughness = 0.3;
        specularStr = 0.2;
    }

    // Red = signs (glossy plastic)
    if (Color.r > 0.5 && Color.g < 0.25 && Color.b < 0.25) {
        roughness = 0.5;
        specularStr = 0.35;
    }

    // Glass (bluish-gray, translucent feel)
    if (Color.b > Color.r && Color.b > 0.4 && saturation < 0.25) {
        roughness = 0.2;
        specularStr = 0.6;
    }

    // ============================================================
    // WALL-MOUNTED FLUORESCENT LIGHTING (brighter, realistic falloff)
    // ============================================================
    vec3 fluorescentColor = vec3(0.98, 0.98, 1.0);  // Bright cool white

    // Distance attenuation (strong near light, gradual falloff)
    float dist = length(lightPos - FragPos);
    float attenuation = 2.5 / (1.0 + 0.15 * dist + 0.05 * dist * dist);

    // Diffuse - Lambert with slight wrap for softer shadows
    float NdotL = max(dot(norm, lightDir), 0.0);
    float wrapDiffuse = max((NdotL + 0.1) / 1.1, 0.0);  // Slight wrap
    float diffuse = mix(NdotL, wrapDiffuse, 0.3);

    // ============================================================
    // SPECULAR - Blinn-Phong
    // ============================================================
    vec3 halfDir = normalize(lightDir + viewDir);
    float NdotH = max(dot(norm, halfDir), 0.0);
    float shininess = mix(4.0, 64.0, 1.0 - roughness);
    float spec = pow(NdotH, shininess) * specularStr;

    // Fresnel for metals
    float fresnel = pow(1.0 - max(dot(norm, viewDir), 0.0), 4.0);
    spec += fresnel * metallic * 0.15;

    // ============================================================
    // AMBIENT OCCLUSION (Screen-space approximation via position)
    // ============================================================
    float ao = 1.0;

    // Subtle darkening near floor
    if (FragPos.y < 0.3) {
        ao *= 0.7 + 0.3 * (FragPos.y / 0.3);
    }

    // Darkening in corners (wall intersections) - adjusted for 4m room
    float cornerDistX = min(abs(FragPos.x + 2.0), abs(FragPos.x - 2.0));
    float cornerDistZ = abs(FragPos.z);
    float cornerDist = min(cornerDistX, cornerDistZ);

    if (cornerDist < 0.3) {
        ao *= 0.75 + 0.25 * (cornerDist / 0.3);
    }

    // Subtle darkening where floor meets wall
    if (FragPos.y < 0.1 && (cornerDistX < 0.2 || cornerDistZ < 0.2)) {
        ao *= 0.75;
    }

    // Light falloff gradient - areas farther from wall light are darker
    float distFromLight = length(vec2(FragPos.x, FragPos.z - 0.2));
    float lightFalloff = 1.0 - smoothstep(1.0, 3.5, distFromLight) * 0.15;
    ao *= lightFalloff;

    // ============================================================
    // COMBINE LIGHTING
    // ============================================================
    vec3 ambient = ambientStrength * ao * Color;
    vec3 diffuseColor = diffuse * attenuation * fluorescentColor * Color;
    vec3 specularColor = spec * attenuation * fluorescentColor;

    // Metallic surfaces tint specular
    specularColor = mix(specularColor, specularColor * Color, metallic * 0.4);

    vec3 result = ambient + diffuseColor + specularColor;

    // ============================================================
    // COLOR GRADING - Realistic indoor fluorescent
    // ============================================================

    // Slight desaturation (10%) for realistic indoor look
    float lum = dot(result, vec3(0.299, 0.587, 0.114));
    result = mix(vec3(lum), result, 0.90);

    // Subtle warm/neutral tint (not too cool)
    result *= vec3(1.0, 0.99, 0.97);

    // Moderate contrast
    result = (result - 0.5) * 1.05 + 0.5;
    result = clamp(result, 0.0, 1.0);

    // Gamma correction
    result = pow(result, vec3(1.0 / 2.2));

    FragColor = vec4(result, 1.0);
}
