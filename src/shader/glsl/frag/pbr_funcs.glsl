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

GK_STRINGIFY(

const float M_PI      = 3.141592653589793;
const float cMinRough = 0.04;

float
max3(vec3 v) {
  return max(max(v.x, v.y), v.z);
}

vec4 sRGBLin(vec4 srgbIn) {
\n#ifndef PRE_SRGB\n
\n#  ifdef SRGB_FAST_APPROXIMATION\n
  vec3 linOut = pow(srgbIn.xyz, vec3(2.2));
\n#  else\n
  vec3 bLess = step(vec3(0.04045), srgbIn.xyz);
  vec3 linOut = mix(srgbIn.xyz / vec3(12.92),
                    pow((srgbIn.xyz + vec3(0.055)) / vec3(1.055), vec3(2.4)),
                    bLess);
\n#  endif\n
  return vec4(linOut, srgbIn.w);
\n#else\n
  return srgbIn;
\n#endif\n
}

/*
 Basic Lambertian diffuse
 Implementation from Lambert's Photometria
   https://archive.org/details/lambertsphotome00lambgoog
 See also [1], Equation 1
 */
vec3
calcDiffuse(vec3 diffuseColor) {
  return diffuseColor / M_PI;
}

/*
 The following equation models the Fresnel reflectance term of the
 spec equation (aka F()) implementation of fresnel from [4], Equation 15
 */
vec3
specRefl(vec3  refl0, vec3  refl90, float VdH) {
  return refl0 + (refl90 - refl0) * pow(clamp(1.0 - VdH, 0.0, 1.0), 5.0);
}

/*
 This calculates the specular geometric attenuation (aka G()),
 where rougher material will reflect less light back to the viewer.
 This implementation is based on [1] Equation 4, and we adopt their
 modifications to alphaRoughness as input as originally proposed in [2].
 */
float
geomOcclusion(float NdL, float NdV, float roughSq) {
  float aL, aV, rr; /* attenuations */

  rr = roughSq * roughSq;
  aL = 2.0 * NdL / (NdL + sqrt(rr + (1.0 - rr) * (NdL * NdL)));
  aV = 2.0 * NdV / (NdV + sqrt(rr + (1.0 - rr) * (NdV * NdV)));

  return aL * aV;
}

/*
 The following equation(s) model the distribution of microfacet normals
 across the area being drawn (aka D())

 Implementation from "Average Irregularity Representation of a
 Roughened Surface for Ray Reflection" by T. S. Trowbridge, and K. P. Reitz

 Follows the distribution function recommended in the SIGGRAPH 2013 course
 notes from EPIC Games [1], Equation 3.
 */
float
microfacetDist(float roughSq, float NdH) {
  float f, rr; /* roughnessSquare */

  rr = roughSq * roughSq;
  f  = (NdH * rr - NdH) * NdH + 1.0;

  return rr / (M_PI * f * f);
}
)
