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

/*
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

#include "../lib/funcs/lerp.glsl"

GK_STRINGIFY(

/*
 Basic Lambertian diffuse
 Implementation from Lambert's Photometria
   https://archive.org/details/lambertsphotome00lambgoog
 See also [1], Equation 1
 */
vec3
calcDiffuse(vec3 diffuseColor) {
  return diffuseColor / cPI;
}

/*
 The following equation models the Fresnel reflectance term of the
 spec equation (aka F()) implementation of fresnel from [4], Equation 15
 */
vec3
surfaceRefl(vec3 F0, float VdH) {
  return F0 + (1.0 - F0) * pow(clamp(1.0 - VdH, 0.0, 1.0), 5.0);
}

/*
 This calculates the specular geometric attenuation (aka G()),
 where rougher material will reflect less light back to the viewer.
 This implementation is based on [1] Equation 4
 */
float
geomOcclusion(float NdL, float NdV, float roughness) {
  float k, Gv, Gl;

  k  = pow(roughness + 1.0, 2.0) / 8.0;
  Gv = NdV / lerp(k, 1.0, NdV);
  Gl = NdL / lerp(k, 1.0, NdL);

  return Gv * Gl;
}

/*
 The following equation(s) model the distribution of microfacet normals
 across the area being drawn (aka D())

 This implementation is based on [1] Equation 3
 */
float
microfacetDist(float NdH, float roughSq) {
  float f, aa;

  aa = roughSq * roughSq;
  f  = NdH * NdH * (aa - 1) + 1.0;

  return aa / (cPI * f * f);
}
)
