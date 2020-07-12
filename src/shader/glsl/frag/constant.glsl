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
  vec4  clr;
\n#ifdef SHADOWMAP\n
  float shadow = shadowCoef();
\n#endif\n

\n#ifdef TRANSP\n
  lightAttn = 1.0;
\n#endif\n
  
  clr =
\n#ifdef SHADOWMAP\n
  shadow * (
\n#endif\n

\n#ifdef EMISSION_TEX\n
  texture(uEmissionTex, EMISSION_TEXCOORD)
\n#elif defined(EMISSION_COLOR)\n
  uEmission
\n#else\n
  vec4(0, 0, 0, 0)
\n#endif\n

\n#ifdef AMBIENT_TEX\n
  + texture(uAmbientTex, AMBIENT_TEXCOORD) * uIAmbient
\n#elif defined(AMBIENT_COLOR)\n
  + uAmbient * uIAmbient
\n#endif\n

\n#ifdef SHADOWMAP\n
  )
\n#endif\n
;

  write(clr);
}
)
