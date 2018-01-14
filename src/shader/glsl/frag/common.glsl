/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

GK_STRINGIFY(
in vec3 vPosition;
in vec4 vPos;
in vec3 vNormal;
in vec3 vEye;

\n#if TEX_COUNT > 0\n
in vec2 vTEXCOORD;
\n#endif\n
\n#if TEX_COUNT > 1\n
in vec2  vTEXCOORD1;
\n#endif\n
\n#if TEX_COUNT > 2\n
in vec2  vTEXCOORD2;
\n#endif\n
\n#if TEX_COUNT > 3\n
in vec2  vTEXCOORD3;
\n#endif\n
\n#if TEX_COUNT > 4\n
in vec2  vTEXCOORD4;
\n#endif\n
\n#if TEX_COUNT > 5\n
in vec2  vTEXCOORD5;
\n#endif\n

\n#ifdef SHADOWMAP\n
in vec2  vSHADOWMAP;
\n#endif\n

\n#ifdef DIFFUSE_TEX\n
uniform sampler2D uDiffuseTex;
\n#elif defined(DIFFUSE_COLOR)\n
uniform vec4      uDiffuse;
\n#endif\n

\n#ifdef EMISSION_TEX\n
uniform sampler2D uEmissionTex;
\n#elif defined(EMISSION_COLOR)\n
uniform vec4      uEmission;
\n#endif\n

\n#ifdef AMBIENT_TEX\n
uniform sampler2D uAmbientTex;
\n#elif defined(AMBIENT_COLOR)\n
uniform vec4      uAmbient;
\n#endif\n

\n#ifdef SPECULAR_TEX\n
uniform sampler2D uSpecularTex;
\n#elif defined(SPECULAR_COLOR)\n
uniform vec4      uSpecular;
\n#endif\n

\n#ifdef REFLECTIVE_TEX\n
uniform sampler2D uReflectiveTex;
\n#elif defined(REFLECTIVE_COLOR)\n
uniform vec4      uReflective;
\n#endif\n

\n#ifdef TRANSP_TEX\n
uniform sampler2D uTransparentTex;
\n#elif defined(TRANSP_COLOR)\n
uniform vec4      uTransparent;
\n#endif\n

\n#if defined(TRANSP_TEX) || defined(TRANSP_COLOR)\n
uniform float     uTransparency;
\n#endif\n

\n#if defined(REFLECTIVE_TEX) || defined(REFLECTIVE_COLOR)\n
uniform float     uReflectivity;
\n#endif\n

\n#ifdef INDEX_OF_REFR\n
uniform float     uIndexOfRefraction;
\n#endif\n

\n#ifdef SHININESS\n
uniform float     uShininess;
\n#endif\n

\n#ifdef SHADOWMAP\n
\n#ifndef SHAD_SPLIT\n

uniform sampler2DShadow      uShadMap;
in vec4                      vShadCoord;

float shadowCoef() {
  return textureProj(uShadMap, vShadCoord);;
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

  if (i >= SHAD_SPLIT)
    return 1.0;

  shadCoord   = uShadMVP[i] * vPos;
  shadCoord.w = shadCoord.z;
  shadCoord.z = float(i);

  return texture(uShadMap, shadCoord);
}

\n#endif\n
\n#endif\n

out vec4 fragColor;
)
