/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

GK_STRINGIFY(
uniform mat4 MVP; // Projection * View * Model matrix
uniform mat4 MV;  // View * Model Matrix
uniform mat4 NM;  // Normal matrix
uniform int  NMU; // Use normal matrix
\n#ifdef POS_WS\n
uniform mat4 M;   // Model matrix
\n#endif\n

\n#ifdef SHADOWMAP\n
\n#ifndef SHAD_SPLIT\n
uniform mat4 uShadMVP;
out vec4     vShadCoord;
\n#endif\n
\n#endif\n

\n#ifndef TEX_COUNT\n
\n#define TEX_COUNT 0\n
\n#endif\n

in vec3 POSITION;
in vec3 NORMAL;

\n#if TEX_COUNT > 0\n
in  vec2   TEXCOORD;
out vec2  vTEXCOORD;
\n#endif\n
\n#if TEX_COUNT > 1\n
in  vec2   TEXCOORD1;
out vec2  vTEXCOORD1;
\n#endif\n
\n#if TEX_COUNT > 2\n
in  vec2   TEXCOORD2;
out vec2  vTEXCOORD2;
\n#endif\n
\n#if TEX_COUNT > 3\n
in  vec2   TEXCOORD3;
out vec2  vTEXCOORD3;
\n#endif\n
\n#if TEX_COUNT > 4\n
in  vec2   TEXCOORD4;
out vec2  vTEXCOORD4;
\n#endif\n
\n#if TEX_COUNT > 5\n
in  vec2   TEXCOORD5;
out vec2  vTEXCOORD5;
\n#endif\n

out vec3 vPos;
out vec3 vNormal;
out vec3 vEye;

\n#ifdef POS_MS\n
\n#endif\n
out vec4 vPosMS;
\n#ifdef POS_WS\n
out vec3 vPosWS;
\n#endif\n

void main() {
  vec4 pos4 = vec4(POSITION, 1.0);
  vPos      = vec3(MV * pos4);
  vEye      = normalize(-vPos);

\n#ifdef POS_WS\n
  vPosWS    = vec3(M  * pos4);
\n#endif\n

\n#ifdef POS_MS\n
  vPosMS    = pos4;
\n#endif\n

  if (NMU == 1)
    vNormal = normalize(vec3(NM * vec4(NORMAL, 0.0)));
  else
    vNormal = normalize(vec3(MV * vec4(NORMAL, 0.0)));

  gl_Position = MVP * pos4;

\n#if TEX_COUNT > 0\n
  vTEXCOORD  = TEXCOORD;
\n#endif\n
\n#if TEX_COUNT > 1\n
  vTEXCOORD1 = TEXCOORD1;
\n#endif\n
\n#if TEX_COUNT > 2\n
  vTEXCOORD2 = TEXCOORD2;
\n#endif\n
\n#if TEX_COUNT > 3\n
  vTEXCOORD3 = TEXCOORD3;
\n#endif\n
\n#if TEX_COUNT > 4\n
  vTEXCOORD4 = TEXCOORD4;
\n#endif\n
\n#if TEX_COUNT > 5\n
  vTEXCOORD5 = TEXCOORD5;
\n#endif\n
  
\n#ifdef SHADOWMAP\n
\n#ifndef SHAD_CUBE\n
\n#ifndef SHAD_SPLIT\n
  vShadCoord = uShadMVP * pos4;
\n#endif\n
\n#endif\n
\n#endif\n
}
)
