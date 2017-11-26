/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

GK_STRINGIFY(

void main() {
  vec4  lightc;
  vec3  L, H;
  float a, Ld, Ls;

\n#ifdef SHADOWMAP\n
  float visibility = 1.0;
  float shadow = texture(uShadowMap, vShadowCoord.xy).r;
  if (shadow < vShadowCoord.z)
    visibility = 0.5;
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

  lightc = light.color * a;

  H  = normalize(L + vEye);
  Ld = max(0.0, dot(vNormal, L));
  Ls = max(0.0, dot(vNormal, H));
  if (Ld == 0.0)
    Ls = 0.0;
  else
    Ls = pow(Ls, uShininess);

  fragColor =
\n#ifdef SHADOWMAP\n
  visibility * (
\n#endif\n

\n#ifdef DIFFUSE_TEX\n
  lightc * texture(uDiffuseTex, DIFFUSE_TEX_COORD) * Ld
\n#elif defined(DIFFUSE_COLOR)\n
  lightc * uDiffuse * Ld
\n#else\n
  lightc * vec4(0, 0, 0, 1)
\n#endif\n

\n#ifdef AMBIENT_TEX\n
  + texture(uAmbientTex, AMBIENT_TEX_COORD)
\n#elif defined(AMBIENT_COLOR)\n
  + uAmbient
\n#endif\n

\n#ifdef EMISSION_TEX\n
  + texture(uEmissionTex, EMISSION_TEX_COORD)
\n#elif defined(EMISSION_COLOR)\n
  + uEmission
\n#endif\n

\n#ifdef SPECULAR_TEX\n
  +  lightc * texture(uSpecularTex, SPECULAR_TEX_COORD) * Ls
\n#elif defined(SPECULAR_COLOR)\n
  +  lightc * uSpecular * Ls
\n#endif\n
                
\n#ifdef SHADOWMAP\n
  )
\n#endif\n
;

}
)
