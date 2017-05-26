/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

GK_STRINGIFY(
\n#version 410 \n
                                                                            
subroutine
vec4
technique(vec4 light, vec3 L);

struct ColorOrTex {
  vec4 color;
};

\n#define AmbientlLight    0x00000001u \n
\n#define DirectionalLight 0x00000002u \n
\n#define PointLight       0x00000003u \n
\n#define SpotLight        0x00000004u \n

struct Material {
  ColorOrTex  emission;
  ColorOrTex  ambient;
  ColorOrTex  specular;
  ColorOrTex  reflective;
  ColorOrTex  transparent;
  ColorOrTex  diffuse;
  float       shininess;
  float       reflectivEyety;
  float       transparency;
  float       indexOfRefraction;
};

struct Light {
  int  enabled;
  uint type;

  vec4 ambient;
  vec4 color;
  vec3 position;
  vec3 direction;

  float cutoffCos;
  float cutoffExp;
  float constAttn;
  float linAttn;
  float quadAttn;
};

subroutine uniform technique effect;

uniform Material material;
uniform Light    lights[16];
uniform int      lightCount;

in vec3 vPosition;
in vec3 vNormal;
in vec3 vEye;

// techniques
subroutine(technique)
vec4
phong(vec4 light, vec3 L) {
  float Ls, Ld;

  Ld = max(0.0, dot(vNormal, L));
  if (Ld == 0.0)
    Ls = 0.0;
  else
    Ls = pow(max(0.0, dot(reflect(-L, vNormal), vEye)),
             material.shininess);

  return material.emission.color
         + light * material.diffuse.color * Ld
         + light * material.specular.color * Ls;
}

subroutine(technique)
vec4
blinn(vec4 light, vec3 L) {
  vec3  H;
  float Ls, Ld;

  H  = normalize(L + vEye);
  Ld = max(0.0, dot(vNormal, L));
  Ls = max(0.0, dot(vNormal, H));
  if (Ld == 0.0)
    Ls = 0.0;
  else
    Ls = pow(Ls, material.shininess);

  return material.emission.color
         + light * material.diffuse.color * Ld
         + light * material.specular.color * Ls;
}

subroutine(technique)
vec4
lambert(vec4 light, vec3 L) {
  float Ld;

  Ld = max(0.0, dot(vNormal, L));
  return material.emission.color
         + light * material.diffuse.color * Ld;
}

subroutine(technique)
vec4
constant(vec4 light, vec3 L) {
  return material.emission.color;
}

// lights
float
point(Light light, inout vec3 L) {
  float dist;
  L    = light.position - vPosition;
  dist = length(L);
  L    = L / dist;
  return 1.0 / (light.constAttn
                + light.linAttn * dist
                + light.quadAttn * dist * dist);
}

float
spot(Light light, inout vec3 L) {
  float dist;
  float spotCos;

  L    = light.position - vPosition;
  dist = length(L);
  L    = L / dist;

  spotCos = dot(light.direction, -L);
  if (spotCos < light.cutoffCos)
    return 0.0;

  return pow(spotCos, light.cutoffExp) / (light.constAttn
                                          + light.linAttn * dist
                                          + light.quadAttn * dist * dist);
}

float
directional(Light light, inout vec3 L) {
  L = -light.direction;
  return 1.0;
}

out vec4 fragColor;

void main() {
  vec4  final, al, amb;
  vec3  L, H;
  float a, Ld, Ls;

  al    = vec4(0.0, 0.0, 0.0, 1.0);
  final = vec4(0.0);

  for (int i = 0; i < lightCount; i++) {
    if (lights[i].enabled == 0)
      continue;

    switch (lights[i].type) {
      case SpotLight:
        a = spot(lights[i], L);
        break;
      case PointLight:
        a = point(lights[i], L);
        break;
      case DirectionalLight:
        a = directional(lights[i], L);
        break;
      default:
        discard;
        return;
    }

    al    += lights[i].ambient * a;
    final += effect(lights[i].color * a, L);
  }

  fragColor = material.ambient.color * al + final;
}
)
