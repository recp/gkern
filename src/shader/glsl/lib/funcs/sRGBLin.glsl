/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

GK_STRINGIFY(

vec4
sRGBLin(vec4 srgbIn) {
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

)
