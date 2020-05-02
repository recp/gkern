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
