/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

GK_STRINGIFY(
\n#version 410 \n

uniform mat4 MVP; // Projection * View * Model matrix

layout(location = 0) in vec3 POSITION;

void main() {
  gl_Position = MVP * vec4(POSITION, 1.0);
}
)
