/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

GK_STRINGIFY(
\n#version 410 \n

uniform mat4 MVP; // Projection * View * Model matrix
uniform mat4 MV;  // View * Model Matrix
uniform mat4 NM;  // Normal matrix
uniform int  NMU; // Use normal matrix

layout(location = 0) in vec3 POSITION;
layout(location = 1) in vec3 NORMAL;

out vec3 vPos;
out vec3 vNormal;
out vec3 vEye;

void main() {
  vec4 pos4 = vec4(POSITION, 1.0);

  vPos = vec3(MV * pos4);
  vEye = normalize(-vPos);

  if (NMU == 1)
    vNormal = normalize(vec3(NM * vec4(NORMAL, 0.0)));
  else
    vNormal = normalize(vec3(MV * vec4(NORMAL, 0.0)));

  gl_Position = MVP * pos4;
}
)
