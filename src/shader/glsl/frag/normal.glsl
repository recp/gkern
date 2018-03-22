/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

/*
 References:
   [1] https://github.com/KhronosGroup/glTF-WebGL-PBR/blob/master/shaders/pbr-frag.glsl
 */

GK_STRINGIFY(
vec3 normal() {

\n#ifndef HAS_TANGENTS\n
  vec3 pdx = dFdx(vPos);
  vec3 pdy = dFdy(vPos);
  vec3 tdx = dFdx(vec3(NORMAL_TEXCOORD, 0.0));
  vec3 tdy = dFdy(vec3(NORMAL_TEXCOORD, 0.0));
  vec3 t   = (tdy.t * pdx - tdx.t * pdy) / (tdx.s * tdy.t - tdy.s * tdx.t);

//\n#ifdef HAS_NORMALS\n
  vec3 ng  = normalize(vNormal);
//\n#else\n
//  vec3 ng  = cross(pdx, pdy);
//\n#endif\n

  t        = normalize(t - ng * dot(ng, t));
  vec3 b   = normalize(cross(ng, t));
  mat3 tbn = mat3(t, b, ng);
\n#else\n
  mat3 tbn = vTBN;
\n#endif\n

\n#ifdef NORMAL_TEX\n
  vec3 n   = texture(uNormalTex, NORMAL_TEXCOORD).rgb;
  n        = normalize(tbn * ((2.0 * n - 1.0) * vec3(uNormalScale, uNormalScale, 1.0)));
\n#else\n
  // The tbn matrix is linearly interpolated, so we need to re-normalize
  vec3 n   = normalize(tbn[2].xyz);
\n#endif\n

  return n;
}
)
