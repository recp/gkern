/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

GK_STRINGIFY(

vec3
lerp(vec3 x, vec3 y, vec3 s) {
  return x + s * (y - x);
}

vec3
lerp(vec3 x, vec3 y, float s) {
  return x + s * (y - x);
}

float
lerp(float x, float y, float s) {
  return x + s * (y - x);
}

)
