/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

GK_STRINGIFY(
layout(location = 0) out float depth;
void main() {
  depth = gl_FragCoord.z;
}
)
