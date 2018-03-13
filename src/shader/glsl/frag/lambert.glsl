/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

GK_STRINGIFY(

void main() {
  vec4  clr, lightc;
  vec3  L;
  float a, Ld;

\n#ifdef SHADOWMAP\n
  float shadow = shadowCoef();
\n#endif\n

  switch (lightType) {
    case SpotLight:
      a = spot(L);
      break;
    case PointLight:
      a = point(L);
      break;
    case DirectionalLight:
      a = directional(L);
      break;
    default:
      discard;
      return;
  }

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
