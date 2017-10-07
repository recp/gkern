/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

GK_STRINGIFY(
struct Material {
\n#ifdef emision_tex\n
  uint emission;
\n#elif defined(emission_color)\n
  vec4 emission;
\n#endif\n

\n#ifdef ambient_tex\n
  uint ambient;
\n#elif defined(ambient_color)\n
  vec4 ambient;
\n#endif\n

\n#ifdef specular_tex\n
  uint specular;
\n#elif defined(specular_color)\n
  vec4 specular;
\n#endif\n

\n#ifdef reflective_tex\n
  uint reflective;
\n#elif defined(reflective_color)\n
  vec4 reflective;
\n#endif\n

\n#ifdef transparent_tex\n
  uint transparent;
\n#elif defined(transparent_color)\n
  vec4 transparent;
\n#endif\n

\n#ifdef diffuse_tex\n
  uint diffuse;
\n#elif defined(diffuse_color)\n
  vec4 diffuse;
\n#endif\n

\n#if defined(transparent_tex) || defined(transparent_color)\n
  float transparency;
\n#endif\n

\n#if defined(reflective_tex) || defined(reflective_color)\n
  float reflectivity;
\n#endif\n

  float shininess;
  float indexOfRefraction;
};

uniform Material mat;

in vec3 vPosition;
in vec3 vNormal;
in vec3 vEye;

\n#ifndef TEX_COUNT\n
\n#define TEX_COUNT 0\n
\n#endif\n

\n#if TEX_COUNT > 0\n
in vec2           vTexCoord[TEX_COUNT];
uniform sampler2D tex2D[TEX_COUNT];
\n#endif\n

out vec4 fragColor;
)
