/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

GK_STRINGIFY(
\n#version 410\n

uniform mat4 MVP; // Projection * View * Model matrix
uniform mat4 MV;  // View * Model Matrix
uniform mat4 NM;  // Normal matrix
uniform int  NMU; // Use normal matrix

layout(location = 0) in vec3 POSITION;
layout(location = 1) in vec3 NORMAL;

\n#ifndef TEX_COUNT\n
\n#  define TEX_COUNT 0\n
\n#endif\n

\n#ifdef TEX_COUNT > 0\n
in vec2 TEXCOORD;
\n#endif\n
\n#ifdef TEX_COUNT > 1\n
in vec2 TEXCOORD1;
\n#endif\n
\n#ifdef TEX_COUNT > 2\n
in vec2 TEXCOORD2;
\n#endif\n
\n#ifdef TEX_COUNT > 3\n
in vec2 TEXCOORD3;
\n#endif\n
\n#ifdef TEX_COUNT > 4\n
in vec2 TEXCOORD4;
\n#endif\n
\n#ifdef TEX_COUNT > 5\n
in vec2 TEXCOORD5;
\n#endif\n

out vec3 vPosition;
out vec3 vNormal;
out vec3 vEye;

\n#ifdef TEX_COUNT > 0\n
out vec2 vTexCoord[TEX_COUNT];
\n#endif\n

void main() {
  vec4 pos4 = vec4(POSITION, 1.0);

  vPosition = vec3(MV * pos4);
  vEye      = normalize(-vPosition);

  if (NMU == 1)
    vNormal = normalize(vec3(NM * vec4(NORMAL, 0.0)));
  else
    vNormal = normalize(vec3(MV * vec4(NORMAL, 0.0)));

  gl_Position = MVP * pos4;

\n#ifdef TEX_COUNT > 0\n
  vTexCoord[0] = TEXCOORD;
\n#endif\n
\n#ifdef TEX_COUNT > 1\n
  vTexCoord[1] = TEXCOORD1;
\n#endif\n
\n#ifdef TEX_COUNT > 2\n
  vTexCoord[2] = TEXCOORD2;
\n#endif\n
\n#ifdef TEX_COUNT > 3\n
  vTexCoord[3] = TEXCOORD3;
\n#endif\n
\n#ifdef TEX_COUNT > 4\n
  vTexCoord[4] = TEXCOORD4;
\n#endif\n
\n#ifdef TEX_COUNT > 5\n
  vTexCoord[5] = TEXCOORD5;
\n#endif\n
}
)
