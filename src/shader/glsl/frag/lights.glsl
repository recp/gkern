/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

GK_STRINGIFY(

float
point(inout vec3 L) {
  float dist;
  L    = light.position - vPos;
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

  L    = light.position - vPos;
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

float
getLight(inout vec3 L) {
  float a;

  switch (lightType) {
    case SpotLight:
      a = spot(L);
      break;
    case PointLight:
      a = point(L);
      break;
    case DirectionalLight:
      a = directional(L);
      break;
    default:
      a = 1.0;
  }

  return a;
}

)
