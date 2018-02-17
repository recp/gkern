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
  vec3  position_ws;
  vec3  direction;

  float cutoffCos;
  float cutoffExp;
  float constAttn;
  float linAttn;
  float quadAttn;
};

uniform Light light;
uniform uint  lightType;

in vec3 vPos;
in vec3 vNormal;
in vec3 vEye;

\n#ifdef POS_MS\n
\n#endif\n
in vec4 vPosMS;
\n#ifdef POS_WS\n
in vec3 vPosWS;
\n#endif\n

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

\n#ifdef SHAD_CUBE\n
uniform samplerCubeShadow    uShadMap;
uniform vec2 uFarNear;

float depthValue(const in vec3 v) {
  vec3 absv = abs(v);
  float z   = max(absv.x, max(absv.y, absv.z));
  return uFarNear.x + uFarNear.y / z;
}

\n#else\n
uniform sampler2DShadow      uShadMap;
in vec4                      vShadCoord;
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

\n#ifdef TRANSP\n
\n#ifdef TRANSP_WBL\n

layout(location = 0) out vec4  accum;
layout(location = 1) out vec4 revealage;

void
transpWrite(vec4 clr /* , vec4 transmit */) {
  float a, b, w, z;

  /*
     clr.a *= 1.0 - clamp((transmit.r + transmit.g + transmit.b) * (1.0 / 3.0),
                          0, 1);
   */

  /* Intermediate terms to be cubed */
  a = min(1.0, clr.a) * 8.0 + 0.01;
  b =-gl_FragCoord.z * 0.95 + 1.0;

  /* If your scene has a lot of content very close to the far plane,
     then include this line (one rsqrt instruction):
     b /= sqrt(1e4 * abs(csZ));
   */

  w = clamp(a * a * a * 1e8 * b * b * b, 1e-2, 3e2);

  /* alternative: equation (10)
     float dz = (1 - gl_FragCoord.z);
     w = clamp(clr.a * max(1e-2, 3e3 * dz * dz * dz), 1e-2, 3e2);
  */

  revealage = vec4(clr.a);
  accum     = vec4(clr.rgb * clr.a, clr.a) * w;
}

\n#endif\n
\n#else\n
out vec4 fragColor;
\n#endif\n

void
write(vec4 clr) {
\n#ifdef TRANSP\n
\n#ifdef TRANSP_WBL\n
  transpWrite(vec4(clr.rgb, 0.7));
  return;
\n#endif\n
\n#else\n
  fragColor = clr;
\n#endif\n
}
)
