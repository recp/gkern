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
   [1] https://github.com/KhronosGroup/glTF-WebGL-PBR/blob/master/shaders/pbr-frag.glsl
 */

/*
 TODO: this will upadated later
 */

GK_STRINGIFY(
vec3 normal() {

\n#if !defined(HAS_TANGENTS) && defined(NORMAL_TEX)\n
  vec3 pdx = dFdx(vPos);
  vec3 pdy = dFdy(vPos);
  vec3 tdx = dFdx(vec3(NORMAL_TEXCOORD, 0.0));
  vec3 tdy = dFdy(vec3(NORMAL_TEXCOORD, 0.0));
  vec3 t   = (tdy.t * pdx - tdx.t * pdy) / (tdx.s * tdy.t - tdy.s * tdx.t);

\n#ifdef HAS_NORMALS\n
  vec3 ng  = normalize(vNormal);
\n#else\n
  vec3 ng  = cross(pdx, pdy);
\n#endif\n

  t        = normalize(t - ng * dot(ng, t));
  vec3 b   = normalize(cross(ng, t));
  mat3 tbn = mat3(t, b, ng);
\n#elif defined(HAS_TBN)\n
  mat3 tbn = vTBN;
\n#endif\n

\n#ifdef NORMAL_TEX\n
  vec3 n   = texture(uNormalTex, NORMAL_TEXCOORD).rgb;
  n        = normalize(tbn * ((2.0 * n - 1.0) * vec3(uNormalScale, uNormalScale, 1.0)));
\n#elif defined(HAS_TBN)\n
  // The tbn matrix is linearly interpolated, so we need to re-normalize
  vec3 n   = normalize(tbn[2].xyz);
\n#else\n
  vec3 n = normalize(vNormal);
\n#endif\n

  if (gl_FrontFacing)
    return n;

  return -n;
}
)
