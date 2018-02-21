/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
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
