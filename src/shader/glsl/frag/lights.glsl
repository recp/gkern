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
    case AmbientLight:
      L = vec3(0);
      a = 1.0;
      break;
    default:
      a = 1.0;
  }

  return a;
}

)
