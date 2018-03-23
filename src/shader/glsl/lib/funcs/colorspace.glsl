/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

GK_STRINGIFY(

const float gamma = 2.2;

float
toLinear(float v) {
  return pow(v, gamma);
}

vec2
toLinear(vec2 v) {
  return pow(v, vec2(gamma));
}

vec3
toLinear(vec3 v) {
  return pow(v, vec3(gamma));
}

vec4
toLinear(vec4 v) {
  return vec4(toLinear(v.rgb), v.a);
}

float
toSRGB(float v) {
  return pow(v, 1.0 / gamma);
}

vec2
toSRGB(vec2 v) {
  return pow(v, vec2(1.0 / gamma));
}

vec3
toSRGB(vec3 v) {
  return pow(v, vec3(1.0 / gamma));
}

vec4
toSRGB(vec4 v) {
  return vec4(toSRGB(v.rgb), v.a);
}

)
