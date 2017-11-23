/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

GK_STRINGIFY(
uniform sampler2D uColorTex;
uniform float     uNear;
uniform float     uFar;

in vec2 vTexCoord;

layout(location = 0) out vec4 color;

void main() {
  /* linearize depth */
  float depth = texture(uColorTex, vTexCoord).x;
  float c     = (2.0 * uNear) / (uFar + uNear - depth * (uFar - uNear));
  color       = vec4(c, c, c, 1);
}
)
