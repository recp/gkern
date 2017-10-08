/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

GK_STRINGIFY(

void main() {
  fragColor =
\n#ifdef EMISSION_TEX\n
  texture(tex2D[mat.emission], vTexCoord[mat.emission])
\n#elif defined(EMISSION_COLOR)\n
  mat.emission
\n#else\n
  vec4(0, 0, 0, 1)
\n#endif\n

\n#ifdef AMBIENT_TEX\n
  + texture(tex2D[mat.ambient], vTexCoord[mat.ambient])
\n#elif defined(AMBIENT_COLOR)\n
  + mat.ambient
\n#endif\n
;

}
)
