/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

GK_STRINGIFY(
layout(location = 0) in vec3 POSITION;
void main() {
  gl_Position = vec4(POSITION, 1.0);
}
)
