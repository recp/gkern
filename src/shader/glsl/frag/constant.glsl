/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
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
  + texture(uAmbientTex, AMBIENT_TEXCOORD)
\n#elif defined(AMBIENT_COLOR)\n
  + uAmbient
\n#endif\n

\n#ifdef SHADOWMAP\n
  )
\n#endif\n
;

  write(clr);
}
)
