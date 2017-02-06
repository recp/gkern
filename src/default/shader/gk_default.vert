/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

"                                                                           \n\
#version 410                                                                \n\
                                                                            \n\
uniform mat4 MVP; // Projection * View * Model matrix                       \n\
uniform mat4 MV;  // View * Model Matrix                                    \n\
uniform mat4 NM;  // Normal matrix                                          \n\
uniform int  NMU; // Use normal matrix                                      \n\
                                                                            \n\
layout(location = 0) in vec3 POSITION;                                      \n\
layout(location = 1) in vec3 NORMAL;                                        \n\
                                                                            \n\
out vec3 vPosition;                                                         \n\
out vec3 vNormal;                                                           \n\
out vec3 vEye;                                                              \n\
                                                                            \n\
void main() {                                                               \n\
  vec4 pos4 = vec4(POSITION, 1.0);                                          \n\
                                                                            \n\
  vPosition = vec3(MV * pos4);                                              \n\
  vEye      = normalize(-vPosition);                                        \n\
                                                                            \n\
  if (NMU == 1)                                                             \n\
    vNormal = normalize(vec3(NM * vec4(NORMAL, 0.0)));                      \n\
  else                                                                      \n\
    vNormal = normalize(vec3(MV * vec4(NORMAL, 0.0)));                      \n\
                                                                            \n\
  gl_Position = MVP * pos4;                                                 \n\
}                                                                           \n\
"