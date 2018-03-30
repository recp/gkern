/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "common.glsl"
#include "normal.glsl"
#include "pbr_common.glsl"
#include "../lib/funcs/max.glsl"

GK_STRINGIFY(

precision highp float;

\n#ifdef SPECGLOSS_TEX\n
uniform sampler2D uSpecGlossTex;
\n#endif\n

\n#ifndef DIFFUSE_COLOR\n
uniform vec4  uDiffuse;
\n#endif\n

\n#ifndef SPECULAR_COLOR\n
uniform vec4  uSpecular;
\n#endif\n

uniform float uGloss;

void main() {
  float G, D, a, gloss, roughSq, NdV, NdL, NdH, LdH, VdH;
  vec3  L, H, N, F, f0, F0, Cdiff, Fspec, Fdiff, lightc, color;
  vec3  specular;

  a = getLight(L);

\n#ifdef TRANSP\n
  lightAttn = a;
\n#endif\n

  lightc = light.color.rgb * a;
  gloss  = uGloss;

\n#ifdef SPECGLOSS_TEX\n
  vec4 sgSample = texture(uSpecGlossTex, SPECGLOSS_TEXCOORD);
  specular      = toLinear(sgSample.rgb);
  gloss         = sgSample.a * gloss;
\n#endif\n

  gloss   = clamp(gloss, 0.0, 1.0);
  roughSq = pow(1.0 - gloss, 2.0);

\n#ifdef DIFFUSE_TEX\n
  vec4 albedo = toLinear(texture(uDiffuseTex, DIFFUSE_TEXCOORD)) * uDiffuse;
\n#else\n
  vec4 albedo = uDiffuse;
\n#endif\n

  Cdiff = albedo.rgb * (1.0 - max(specular));
  F0    = specular;

  N     =  normal();
  H     =  normalize(L + vEye);
  NdV   =  clamp(dot(N, vEye), 0.001, 1.0);
  NdL   =  clamp(dot(N, L),    0.001, 1.0);
  NdH   =  clamp(dot(N, H),    0.0,   1.0);
  LdH   =  clamp(dot(L, H),    0.0,   1.0);
  VdH   =  clamp(dot(vEye, H), 0.0,   1.0);

  /* Calculate the shading terms for the microfacet specular shading model */
  F = surfaceRefl(F0, VdH);
  G = geomOcclusion(NdL, NdV, roughSq);
  D = microfacetDist(NdH, roughSq);

  /* Schlick BRDF */
  Fdiff = (1.0 - F) * calcDiffuse(Cdiff);
  Fspec = F * G * D / (4.0 * NdL * NdV);

  /*
   Obtain final intensity as reflectance (BRDF) scaled by the energy of
   the light (cosine law)
   */
  color = NdL * lightc * (Fdiff + Fspec);

  applyOcclusion(color);
  applyEmission(color);

  write(vec4(toSRGB(color), albedo.a));
}
)
