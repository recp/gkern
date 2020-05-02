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

\n#ifdef SHADOWMAP\n
\n#ifndef SHAD_SPLIT\n

\n#ifdef SHAD_CUBE\n
uniform samplerCubeShadow uShadMap;
uniform vec2              uFarNear;

float depthValue(const in vec3 v) {
  vec3 absv = abs(v);
  float z   = max(absv.x, max(absv.y, absv.z));
  return uFarNear.x + uFarNear.y / z;
}

\n#else\n
uniform sampler2DShadow   uShadMap;
in vec4                   vShadCoord;
\n#endif\n

float shadowCoef() {
  \n#ifdef SHAD_CUBE\n
  vec3  L;
  float d;

  L = vPosWS - light.position_ws;
  d = depthValue(L);

  return texture(uShadMap, vec4(L, d));
  \n#else\n
    return textureProj(uShadMap, vShadCoord);
  \n#endif\n
}

\n#else\n

uniform sampler2DArrayShadow uShadMap;
uniform mat4                 uShadMVP[SHAD_SPLIT];
uniform float                uShadDist[SHAD_SPLIT];

float shadowCoef() {
  vec4 shadCoord;
  int  i;

  for (i = 0; i < SHAD_SPLIT; i++) {
    if (gl_FragCoord.z < uShadDist[i])
      break;
  }

  shadCoord     = uShadMVP[i] * vPosMS;
  shadCoord.xyw = (shadCoord / shadCoord.w).xyz;
  shadCoord.z   = float(i);

  return texture(uShadMap, shadCoord);
}

\n#endif\n
\n#endif\n

)
