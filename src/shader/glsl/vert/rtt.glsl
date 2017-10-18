/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

GK_STRINGIFY(
layout(location = 0) in vec2 POSITION;
layout(location = 1) in vec2 TEXCOORD;

out vec2 vTexCoord;
void main() {
  vTexCoord   = TEXCOORD;
  gl_Position = vec4(POSITION, 0.0, 1.0);
}
)
