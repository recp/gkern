/*
 * This file is part of the gk project (https://github.com/recp/gk)
 * Copyright (c) Recep Aslantas.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
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
