/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

GK_STRINGIFY(

void main() {
\n#ifdef SHADOWMAP\n
  float shadow = 0.5 + 0.5 * textureProj(uShadowMap, vShadowCoord);
\n#endif\n

  fragColor =
\n#ifdef SHADOWMAP\n
  shadow * (
\n#endif\n

\n#ifdef EMISSION_TEX\n
  texture(uEmissionTex, EMISSION_TEX_COORD)
\n#elif defined(EMISSION_COLOR)\n
  uEmission
\n#else\n
  vec4(0, 0, 0, 1)
\n#endif\n

\n#ifdef AMBIENT_TEX\n
  + texture(uAmbientTex, AMBIENT_TEX_COORD)
\n#elif defined(AMBIENT_COLOR)\n
  + uAmbient
\n#endif\n

\n#ifdef SHADOWMAP\n
  )
\n#endif\n
;

}
)
