/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

/*
 References:
 [0] https://github.com/SFML/SFML/wiki/Source:-Radial-Gradient-Shader
 */

GK_STRINGIFY(

out vec4 fragColor;

uniform vec2 uSize;

/* TODO: maybe uniform */
vec4 bgcolor1 = vec4(0.1, 0.1, 0.1, 1.0);
vec4 bgcolor2 = vec4(0.19803921569, 0.2137254902, 0.2490196078, 1.0);

void main() {
  vec2  centerFlipped = uSize * 0.5;
  float radius        = centerFlipped.y;

  vec2  center        = vec2(centerFlipped.x, uSize.y - centerFlipped.y);
  vec2  p             = (gl_FragCoord.xy - center) / radius;
  float r             = sqrt(dot(p, p));

  fragColor           = mix(bgcolor2, bgcolor1, r);
}
)
