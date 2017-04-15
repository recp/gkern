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
                                                                            \n\
layout(location = 0) in vec3 POSITION;                                      \n\
                                                                            \n\
void main() {                                                               \n\
  gl_Position = MVP * vec4(POSITION, 1.0);                                  \n\
}                                                                           \n\
"
