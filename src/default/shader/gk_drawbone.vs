/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

GK_STRINGIFY(
\n#version 410 \n

uniform mat4 MV;
layout(std140) uniform JointBlock {
  mat4 uBones[255];
};

void main() {
  gl_Position = MV * uBones[gl_VertexID] * vec4(1.0, 0.0, 0.0, 1.0);
}
)
