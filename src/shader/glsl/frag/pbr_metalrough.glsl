/*
 * This file is part of the gk project (https://github.com/recp/gk)
 * Copyright (c) Recep Aslantas.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "common.glsl"
#include "normal.glsl"
#include "pbr_common.glsl"

GK_STRINGIFY(

precision highp float;

\n#ifdef ALBEDO_TEX\n
uniform sampler2D uAlbedoTex;
\n#endif\n

\n#ifdef METALROUGH_TEX\n
uniform sampler2D uMetalRoughTex;
\n#endif\n

uniform vec2 uMetalRough;
uniform vec4 uAlbedo;

void main() {
  float G, D, a, metal, rough, roughSq, NdV, NdL, NdH, LdH, VdH;
  vec3  L, H, N, F, f0, F0, Cdiff, Fspec, Fdiff, lightc, color;

  a = getLight(L);

\n#ifdef TRANSP\n
  lightAttn = a;
\n#endif\n

  lightc = light.color.rgb * a;
  metal  = uMetalRough.x;
  rough  = uMetalRough.y;

\n#ifdef METALROUGH_TEX\n
  vec4 mrSample = texture(uMetalRoughTex, METALROUGH_TEXCOORD);
  rough         = mrSample.g * rough;
  metal         = mrSample.b * metal;
\n#endif\n

  metal   = clamp(metal, 0.0,       1.0);
  rough   = clamp(rough, cMinRough, 1.0);
  roughSq = rough * rough;

\n#ifdef ALBEDO_TEX\n
  vec4 albedo = toLinear(texture(uAlbedoTex, ALBEDO_TEXCOORD)) * uAlbedo;
\n#else\n
  vec4 albedo = uAlbedo;
\n#endif\n

  Cdiff = lerp(albedo.rgb * (1.0 - cDielectricSpecular.r), cBlack, metal);
  F0    = lerp(cDielectricSpecular, albedo.rgb, metal);

  N     = normal();
  H     = normalize(L + vEye);
  NdV   = clamp(dot(N, vEye), 0.001, 1.0);
  NdL   = clamp(dot(N, L),    0.001, 1.0);
  NdH   = clamp(dot(N, H),    0.0,   1.0);
  LdH   = clamp(dot(L, H),    0.0,   1.0);
  VdH   = clamp(dot(vEye, H), 0.0,   1.0);

  /* Calculate the shading terms for the microfacet specular shading model */
  F     = surfaceRefl(F0, VdH);
  G     = geomOcclusion(NdL, NdV, roughSq);
  D     = microfacetDist(NdH, roughSq);

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
