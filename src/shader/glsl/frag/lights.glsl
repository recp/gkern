/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

GK_STRINGIFY(
\n#define AmbientlLight    0x00000001u \n
\n#define DirectionalLight 0x00000002u \n
\n#define PointLight       0x00000003u \n
\n#define SpotLight        0x00000004u \n
struct Light {
  vec4  ambient;
  vec4  color;
  vec3  position;
  vec3  direction;

  float cutoffCos;
  float cutoffExp;
  float constAttn;
  float linAttn;
  float quadAttn;
};


uniform Light light;
uniform uint  lightType;

// lights

float
point(inout vec3 L) {
  float dist;
  L    = light.position - vPosition;
  dist = length(L);
  L    = L / dist;
  return 1.0 / (light.constAttn
                + light.linAttn * dist
                + light.quadAttn * dist * dist);
}

float
spot(inout vec3 L) {
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
directional(inout vec3 L) {
  L = -light.direction;
  return 1.0;
}
)
