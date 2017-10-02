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

  a      = lightTechn(L);
  lightc = light.color * a;

  H  = normalize(L + vEye);
  Ld = max(0.0, dot(vNormal, L));
  Ls = max(0.0, dot(vNormal, H));
  if (Ld == 0.0)
    Ls = 0.0;
  else
    Ls = pow(Ls, material.shininess);

  fragColor =
#ifdef diffuse_tex
  lightc * texture(tex2D[material.diffuse], vTexCoord[material.diffuse]) * Ld
#elif defined(diffuse_color)
  lightc * material.diffuse * Ld
#else
  lightc * vec4(0, 0, 0, 1)
#endif

#ifdef ambient_tex
  + texture(tex2D[material.ambient], vTexCoord[material.ambient])
#elif defined(ambient_color)
  + material.ambient
#endif

#ifdef emission_tex
  + texture(tex2D[material.emission], vTexCoord[material.emission])
#elif defined(emission_color)
  + material.emission
#endif

#ifdef specular_tex
  +  lightc * texture(tex2D[material.specular], vTexCoord[material.specular]) * Ls
#elif defined(specular_color)
  +  lightc * material.specular * Ls
#endif
;

}
)
