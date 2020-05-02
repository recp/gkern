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

#include "common.glsl"

GK_STRINGIFY(

void main() {
  vec4  clr, lightc;
  vec3  L;
  float a, Ld;

\n#ifdef SHADOWMAP\n
  float shadow = shadowCoef();
\n#endif\n

  a = getLight(L);

\n#ifdef TRANSP\n
   lightAttn = a;
\n#endif\n

  lightc = vec4(light.color.rgb, 1.0) * a;

\n#ifndef TRANSP\n
  Ld     = max(0.0, dot(vNormal, L));
\n#else\n
  vec3 N;

  N  = vNormal * sign(dot(vNormal, L));
  Ld = max(0.0, dot(N, L));
\n#endif\n

  clr =
\n#ifdef SHADOWMAP\n
  shadow * (
\n#endif\n

\n#ifdef DIFFUSE_TEX\n
  lightc * texture(uDiffuseTex, DIFFUSE_TEXCOORD) * Ld
\n#elif defined(DIFFUSE_COLOR)\n
  lightc * uDiffuse * Ld
\n#else\n
  lightc * vec4(1, 1, 1, 1) * Ld
\n#endif\n

\n#ifdef AMBIENT_TEX\n
  + texture(uAmbientTex, AMBIENT_TEXCOORD)
\n#elif defined(AMBIENT_COLOR)\n
  + uAmbient
\n#endif\n

\n#ifdef EMISSION_TEX\n
  + texture(uEmissionTex, EMISSION_TEXCOORD)
\n#elif defined(EMISSION_COLOR)\n
  + uEmission
\n#endif\n

\n#ifdef SHADOWMAP\n
  )
\n#endif\n

;

  write(clr);
}
)
