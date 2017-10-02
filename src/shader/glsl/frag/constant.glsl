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
  float a;

  a      = lightTechn(L);
  lightc = light.color * a;

  fragColor =
#ifdef ambient_tex
  texture(tex2D[material.ambient], vTexCoord[material.ambient])
#elif defined(ambient_color)
  material.ambient
#endif

#ifdef emission_tex
  + texture(tex2D[material.emission], vTexCoord[material.emission])
#elif defined(emission_color)
  + material.emission
#endif
;

}
)
