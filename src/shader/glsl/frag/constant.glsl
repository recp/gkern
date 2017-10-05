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

  fragColor =
\n#ifdef ambient_tex\n
  texture(tex2D[material.ambient], vTexCoord[material.ambient])
\n#elif defined(ambient_color)\n
  material.ambient
\n#endif\n

\n#ifdef emission_tex\n
  + texture(tex2D[material.emission], vTexCoord[material.emission])
\n#elif defined(emission_color)\n
  + material.emission
\n#endif\n
;

}
)
