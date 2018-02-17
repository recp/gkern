/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

/*
 http://casual-effects.blogspot.com.tr/2015/03/implemented-weighted-blended
 -order.html
 */

GK_STRINGIFY(
uniform sampler2D uAccum;  /* sum(rgb * a, a) */
uniform sampler2D uReveal; /* prod(1 - a)     */

float max4(vec4 v) {
  return max(max(v.x, v.y), max(v.z, v.w));
}

layout(location = 0) out vec4 fragcolor;

void main() {
  float revealage;
  vec4  accum;
  vec3  averageColor;
  ivec2 C;

  C         = ivec2(gl_FragCoord.xy);
  revealage = texelFetch(uReveal, C, 0).r;

  /* Save the blending and color texture fetch cost */
  if (revealage > 1.0)
    discard;

  accum = texelFetch(uAccum, C, 0);

  /* Suppress overflow */
  if (isinf(max4(abs(accum))))
    accum.rgb = vec3(accum.a);

  averageColor = accum.rgb / max(accum.a, 0.00001);

  fragcolor = vec4(averageColor, 1.0 - revealage);
}
)
