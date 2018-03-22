/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

/*
 Initially glTF-WebGL-PBR's reference shaders are borrowed and modified

 References:
 [0] KhronosGroup's glTF-WebGL-PBR reference shader
 https://github.com/KhronosGroup/glTF-WebGL-PBR/blob/master/shaders/pbr-frag.glsl
 [1] Real Shading in Unreal Engine 4
 http://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf
 [2] Physically Based Shading at Disney
 http://blog.selfshadow.com/publications/s2012-shading-course/burley/s2012_pbs_disney_brdf_notes_v3.pdf
 [3] README.md - Environment Maps
 https://github.com/KhronosGroup/glTF-WebGL-PBR/#environment-maps
 [4] "An Inexpensive BRDF Model for Physically based Rendering" by Christophe Schlick
 https://www.cs.virginia.edu/~jdl/bib/appearance/analytic%20models/schlick94b.pdf
 */

#include "common.glsl"
#include "normal.glsl"
#include "pbr_funcs.glsl"

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
  float metal, rough, a, alphaRough, reflectance, reflectance90,
        NdV, NdL, NdH, LdH, VdH, G, D;

  vec3 diffuseColor, specularColor, specEnvR0, specEnvR90,
       reflection, diffuseContrib, specContrib,
       L, H, N, F, f0, lightc, color;

  a      = getLight(L);
  lightc = light.color.rgb * a;
  metal  = uMetalRough.x;
  rough  = uMetalRough.y;

\n#ifdef METALROUGH_TEX\n
  vec4 mrSample = texture(uMetalRoughTex, METALROUGH_TEXCOORD);
  rough         = mrSample.g * rough;
  metal         = mrSample.b * metal;
\n#endif\n

  rough = clamp(rough, cMinRough, 1.0);
  metal = clamp(metal, 0.0,       1.0);

\n#ifdef ALBEDO_TEX\n
  vec4 albedo = sRGBLin(texture(uAlbedoTex, ALBEDO_TEXCOORD)) * uAlbedo;
\n#else\n
  vec4 albedo = uAlbedo;
\n#endif\n

  alphaRough    = rough * rough;
  f0            = vec3(0.04);
  diffuseColor  = albedo.rgb * (vec3(1.0) - f0);
  diffuseColor *= 1.0 - metal;
  specularColor = mix(f0, albedo.rgb, metal);

  /* Compute reflectance */
  reflectance   = max3(specularColor);

  /*
   For typical incident reflectance range (between 4% to 100%) set the grazing
   reflectance to 100% for typical fresnel effect.

   For very low reflectance range on highly diffuse objects (below 4%),
   incrementally reduce grazing reflecance to 0%.
   */
  reflectance90 = clamp(reflectance * 25.0, 0.0, 1.0);
  specEnvR0     = specularColor.rgb;
  specEnvR90    = vec3(1.0, 1.0, 1.0) * reflectance90;

  N          = normal();
  H          = normalize(L + vEye);
  NdV        = abs(dot(N, vEye)) + 0.001;
  NdL        = clamp(dot(N, L),    0.001, 1.0);
  NdH        = clamp(dot(N, H),    0.0,   1.0);
  LdH        = clamp(dot(L, H),    0.0,   1.0);
  VdH        = clamp(dot(vEye, H), 0.0,   1.0);
  reflection = -normalize(reflect(vEye, N));

  /* Calculate the shading terms for the microfacet specular shading model */
  F = specRefl(specEnvR0, specEnvR90, VdH);
  G = geomOcclusion(NdL, NdV, alphaRough);
  D = microfacetDist(alphaRough, NdH);

  /* Calculation of analytical lighting contribution */
  diffuseContrib = (1.0 - F) * calcDiffuse(diffuseColor);
  specContrib    = F * G * D / (4.0 * NdL * NdV);

  /*
   Obtain final intensity as reflectance (BRDF) scaled by the energy of
   the light (cosine law)
   */
  color = NdL * lightc * (diffuseContrib + specContrib);

\n#ifdef OCCLUSION_TEX\n
  float ao = texture(uOcclusionTex, OCCLUSION_TEXCOORD).r;
  color    = mix(color, color * ao, uOcclusionStrength);
\n#endif\n

\n#ifdef EMISSION_TEX\n
  color += sRGBLin(texture(uEmissionTex, EMISSION_TEXCOORD)).rgb;
\n#elif defined(EMISSION_COLOR)\n
  color += uEmission;
\n#elif defined(EMISSION_TEXCOLOR)\n
  color += sRGBLin(texture(uEmissionTex, EMISSION_TEXCOORD)).rgb * uEmission;
\n#endif\n

  write(vec4(pow(color, vec3(1.0/2.2)), albedo.a));
}
)
