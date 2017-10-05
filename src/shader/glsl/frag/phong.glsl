/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

GK_STRINGIFY(
void main() {
  vec4  lightc;
  vec3  L;
  float a, Ld, Ls;

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

  Ld = max(0.0, dot(vNormal, L));
  if (Ld == 0.0)
    Ls = 0.0;
  else
    Ls = pow(max(0.0, dot(reflect(-L, vNormal), vEye)),
             material.shininess);

  fragColor =
\n#ifdef diffuse_tex\n
  lightc * texture(tex2D[material.diffuse], vTexCoord[material.diffuse]) * Ld
\n#elif defined(diffuse_color)\n
  lightc * material.diffuse * Ld
\n#else\n
  lightc * vec4(0, 0, 0, 1)
\n#endif\n

\n#ifdef ambient_tex\n
  + texture(tex2D[material.ambient], vTexCoord[material.ambient])
\n#elif defined(ambient_color)\n
  + material.ambient
\n#endif\n

\n#ifdef emission_tex\n
  + texture(tex2D[material.emission], vTexCoord[material.emission])
\n#elif defined(emission_color)\n
  + material.emission
\n#endif\n

\n#ifdef specular_tex\n
  +  lightc * texture(tex2D[material.specular], vTexCoord[material.specular]) * Ls
\n#elif defined(specular_color)\n
  +  lightc * material.specular * Ls
\n#endif\n
;

}
)
