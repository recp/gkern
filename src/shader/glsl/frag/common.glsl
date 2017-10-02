/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

GK_STRINGIFY(
\n#version 410 \n

struct Material {
\n#ifdef emision_tex\n
  uint emission;
\n#elif emission_color\n
  vec4 emission;
\n#endif

\n#ifdef ambient_tex\n
  uint ambient;
\n#elif ambient_color\n
  vec4 ambient;
\n#endif

\n#ifdef specular_tex\n
  uint specular;
\n#elif specular_color\n
  vec4 specular;
\n#endif

\n#ifdef reflective_tex\n
  uint reflective;
\n#elif reflective_color\n
  vec4 reflective;
\n#endif

\n#ifdef transparent_tex\n
  uint transparent;
\n#elif transparent_color\n
  vec4 transparent;
\n#endif

\n#ifdef diffuse_tex\n
  uint diffuse;
\n#elif diffuse_color\n
  vec4 diffuse;
\n#endif

\n#if defined(transparent_tex) || defined(transparent_color)\n
  float transparency;
\n#endif

\n#if defined(reflective_tex) || defined(reflective_color)\n
  float reflectivity;
\n#endif

  float shininess;
  float indexOfRefraction;
};

uniform Material material;

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
