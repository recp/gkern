/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

GK_STRINGIFY(
uniform mat4 MVP; /* Projection * View * Model matrix */
uniform mat4 MV;  /* View * Model Matrix              */
uniform mat4 NM;  /* Normal matrix                    */
uniform int  NMU; /* Use normal matrix                */
\n#ifdef POS_WS\n
uniform mat4 M;   /* Model matrix                     */
\n#endif\n
uniform mat4 M;
\n#ifdef SHADOWMAP\n
\n#ifndef SHAD_SPLIT\n
uniform mat4 uShadMVP;
out vec4     vShadCoord;
\n#endif\n
\n#endif\n

\n#ifndef TEX_COUNT\n
\n#define TEX_COUNT 0\n
\n#endif\n

\n#define TEX_OUT0   vTEXCOORD      = TEXCOORD;\n
\n#define TEX_OUT(X) vTEXCOORD ## X = TEXCOORD ## X;\n
\n#define TEX_OUT_DEF0   in vec2 TEXCOORD;      out vec2 vTEXCOORD;\n
\n#define TEX_OUT_DEF(X) in vec2 TEXCOORD ## X; out vec2 vTEXCOORD ## X;\n

in vec3 POSITION;
in vec3 NORMAL;

\n#if TEX_COUNT > 0\n  TEX_OUT_DEF0    \n#endif\n
\n#if TEX_COUNT > 1\n  TEX_OUT_DEF(1)  \n#endif\n
\n#if TEX_COUNT > 2\n  TEX_OUT_DEF(2)  \n#endif\n
\n#if TEX_COUNT > 3\n  TEX_OUT_DEF(3)  \n#endif\n
\n#if TEX_COUNT > 4\n  TEX_OUT_DEF(4)  \n#endif\n
\n#if TEX_COUNT > 5\n  TEX_OUT_DEF(5)  \n#endif\n

\n#ifdef JOINT_COUNT\n
layout (std140) uniform JointBlock {
  mat4 uJoints[JOINT_COUNT];
};

in uvec4 JOINTS;
in vec4  JOINTWEIGHTS;
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
  vec4 pos4, norm4;

  pos4  = vec4(POSITION, 1.0);
  norm4 = vec4(NORMAL,   0.0);
  vPos  = vec3(MV * pos4);
  vEye  = normalize(-vPos);

\n#ifdef JOINT_COUNT\n
  mat4 skinMat;

  skinMat = uJoints[JOINTS.x] * JOINTWEIGHTS.x
          + uJoints[JOINTS.y] * JOINTWEIGHTS.y
          + uJoints[JOINTS.z] * JOINTWEIGHTS.z
          + uJoints[JOINTS.w] * JOINTWEIGHTS.w;

  pos4  = inverse(M) * skinMat * pos4;
  norm4 = inverse(M) * skinMat * norm4;
\n#endif\n

\n#ifdef POS_WS\n
  vPosWS = vec3(M * pos4);
\n#endif\n

\n#ifdef POS_MS\n
  vPosMS = pos4;
\n#endif\n

  if (NMU == 1)
    vNormal = normalize(vec3(NM * norm4));
  else
    vNormal = normalize(vec3(MV * norm4));

  gl_Position = MVP * pos4;

\n#if TEX_COUNT > 0\n  TEX_OUT0    \n#endif\n
\n#if TEX_COUNT > 1\n  TEX_OUT(1)  \n#endif\n
\n#if TEX_COUNT > 2\n  TEX_OUT(2)  \n#endif\n
\n#if TEX_COUNT > 3\n  TEX_OUT(3)  \n#endif\n
\n#if TEX_COUNT > 4\n  TEX_OUT(4)  \n#endif\n
\n#if TEX_COUNT > 5\n  TEX_OUT(5)  \n#endif\n

\n#ifdef SHADOWMAP\n
\n#ifndef SHAD_CUBE\n
\n#ifndef SHAD_SPLIT\n
  vShadCoord = uShadMVP * pos4;
\n#endif\n
\n#endif\n
\n#endif\n
}
)
