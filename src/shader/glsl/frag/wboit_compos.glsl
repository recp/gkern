/*
 * This file is part of the gk project (https://github.com/recp/gk)
 * Copyright (c) Recep Aslantas.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
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
  if (revealage >= 1.0)
    discard;

  accum = texelFetch(uAccum, C, 0);

  /* Suppress overflow */
  if (isinf(max4(abs(accum))))
    accum.rgb = vec3(accum.a);

  averageColor = accum.rgb / max(accum.a, 0.00001);

  fragcolor = vec4(averageColor, revealage);
}
)
