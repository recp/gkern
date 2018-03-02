/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

GK_STRINGIFY(

out vec4 fragColor;

uniform sampler2D uColorTex;

in vec2 vTexCoord;

void main() {
  fragColor = texture(uColorTex, vTexCoord);
}
)
