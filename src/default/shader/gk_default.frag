/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

"                                                                           \n\
#version 410                                                                \n\
                                                                            \n\
subroutine                                                                  \n\
vec4                                                                        \n\
technique(vec4 light, vec3 L);                                              \n\
                                                                            \n\
struct ColorOrTex {                                                         \n\
  vec4 color;                                                               \n\
};                                                                          \n\
                                                                            \n\
#define AmbientlLight    0x00000001u                                        \n\
#define DirectionalLight 0x00000002u                                        \n\
#define PointLight       0x00000003u                                        \n\
#define SpotLight        0x00000004u                                        \n\
                                                                            \n\
struct Material {                                                           \n\
  ColorOrTex  emission;                                                     \n\
  ColorOrTex  ambient;                                                      \n\
  ColorOrTex  specular;                                                     \n\
  ColorOrTex  reflective;                                                   \n\
  ColorOrTex  transparent;                                                  \n\
  ColorOrTex  diffuse;                                                      \n\
  float       shininess;                                                    \n\
  float       reflectivEyety;                                               \n\
  float       transparency;                                                 \n\
  float       indexOfRefraction;                                            \n\
};                                                                          \n\
                                                                            \n\
struct Light {                                                              \n\
  int  enabled;                                                             \n\
  uint type;                                                                \n\
                                                                            \n\
  vec4 ambient;                                                             \n\
  vec4 color;                                                               \n\
  vec3 position;                                                            \n\
  vec3 direction;                                                           \n\
                                                                            \n\
  float cutoffCos;                                                          \n\
  float cutoffExp;                                                          \n\
  float constAttn;                                                          \n\
  float linAttn;                                                            \n\
  float quadAttn;                                                           \n\
};                                                                          \n\
                                                                            \n\
subroutine uniform technique effect;                                        \n\
                                                                            \n\
uniform Material material;                                                  \n\
uniform Light    lights[16];                                                \n\
uniform int      lightCount;                                                \n\
                                                                            \n\
in vec3 vPosition;                                                          \n\
in vec3 vNormal;                                                            \n\
in vec3 vEye;                                                               \n\
                                                                            \n\
// techniques                                                               \n\
subroutine(technique)                                                       \n\
vec4                                                                        \n\
phong(vec4 light, vec3 L) {                                                 \n\
  float Ls, Ld;                                                             \n\
                                                                            \n\
  Ld = max(0.0, dot(vNormal, L));                                           \n\
  if (Ld == 0.0)                                                            \n\
    Ls = 0.0;                                                               \n\
  else                                                                      \n\
    Ls = pow(max(0.0, dot(reflect(-L, vNormal), vEye)),                     \n\
             material.shininess);                                           \n\
                                                                            \n\
  return material.emission.color                                            \n\
  + light * material.diffuse.color * Ld                                     \n\
  + light * material.specular.color * Ls;                                   \n\
}                                                                           \n\
                                                                            \n\
subroutine(technique)                                                       \n\
vec4                                                                        \n\
blinn(vec4 light, vec3 L) {                                                 \n\
  vec3  H;                                                                  \n\
  float Ls, Ld;                                                             \n\
                                                                            \n\
  H  = normalize(L + vEye);                                                 \n\
  Ld = max(0.0, dot(vNormal, L));                                           \n\
  Ls = max(0.0, dot(vNormal, H));                                           \n\
  if (Ld == 0.0)                                                            \n\
    Ls = 0.0;                                                               \n\
  else                                                                      \n\
    Ls = pow(Ls, material.shininess);                                       \n\
                                                                            \n\
  return material.emission.color                                            \n\
  + light * material.diffuse.color * Ld                                     \n\
  + light * material.specular.color * Ls;                                   \n\
}                                                                           \n\
                                                                            \n\
subroutine(technique)                                                       \n\
vec4                                                                        \n\
lambert(vec4 light, vec3 L) {                                               \n\
  float Ld;                                                                 \n\
                                                                            \n\
  Ld = max(0.0, dot(vNormal, L));                                           \n\
  return material.emission.color                                            \n\
  + light * material.diffuse.color * Ld;                                    \n\
}                                                                           \n\
                                                                            \n\
subroutine(technique)                                                       \n\
vec4                                                                        \n\
constant(vec4 light, vec3 L) {                                              \n\
  return material.emission.color;                                           \n\
}                                                                           \n\
                                                                            \n\
// lights                                                                   \n\
float                                                                       \n\
point(Light light, inout vec3 L) {                                          \n\
  float dist;                                                               \n\
  L    = light.position - vPosition;                                        \n\
  dist = length(L);                                                         \n\
  L    = L / dist;                                                          \n\
  return 1.0 / (light.constAttn                                             \n\
                + light.linAttn * dist                                      \n\
                + light.quadAttn * dist * dist);                            \n\
}                                                                           \n\
                                                                            \n\
float                                                                       \n\
spot(Light light, inout vec3 L) {                                           \n\
  float dist;                                                               \n\
  float spotCos;                                                            \n\
                                                                            \n\
  L    = light.position - vPosition;                                        \n\
  dist = length(L);                                                         \n\
  L    = L / dist;                                                          \n\
                                                                            \n\
  spotCos = dot(light.direction, -L);                                       \n\
  if (spotCos < light.cutoffCos)                                            \n\
    return 0.0;                                                             \n\
                                                                            \n\
  return pow(spotCos, light.cutoffExp) / (light.constAttn                   \n\
                                          + light.linAttn * dist            \n\
                                          + light.quadAttn * dist * dist);  \n\
}                                                                           \n\
                                                                            \n\
float                                                                       \n\
directional(Light light, inout vec3 L) {                                    \n\
  L = -light.direction;                                                     \n\
  return 1.0;                                                               \n\
}                                                                           \n\
                                                                            \n\
out vec4 fragColor;                                                         \n\
                                                                            \n\
void main() {                                                               \n\
  vec4  final, al, amb;                                                     \n\
  vec3  L, H;                                                               \n\
  float a, Ld, Ls;                                                          \n\
                                                                            \n\
  al    = vec4(0.0, 0.0, 0.0, 1.0);                                         \n\
  final = vec4(0.0);                                                        \n\
                                                                            \n\
  for (int i = 0; i < lightCount; i++) {                                    \n\
    if (lights[i].enabled == 0)                                             \n\
      continue;                                                             \n\
                                                                            \n\
    switch (lights[i].type) {                                               \n\
      case SpotLight:                                                       \n\
        a = spot(lights[i], L);                                             \n\
        break;                                                              \n\
      case PointLight:                                                      \n\
        a = point(lights[i], L);                                            \n\
        break;                                                              \n\
      case DirectionalLight:                                                \n\
        a = directional(lights[i], L);                                      \n\
        break;                                                              \n\
      default:                                                              \n\
        discard;                                                            \n\
        return;                                                             \n\
    }                                                                       \n\
                                                                            \n\
    al    += lights[i].ambient * a;                                         \n\
    final += effect(lights[i].color * a, L);                                \n\
  }                                                                         \n\
                                                                            \n\
  fragColor = material.ambient.color * al + final;                          \n\
}                                                                           \n\
"