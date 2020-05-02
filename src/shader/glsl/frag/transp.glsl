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

GK_STRINGIFY(

\n#ifdef TRANSP\n

\n#ifdef TRANSP_TEX\n
uniform sampler2D uTransparentTex;
\n#elif defined(TRANSP_COLOR)\n
uniform vec4      uTransparent;
\n#endif\n

\n#if defined(TRANSP_TEX) || defined(TRANSP_COLOR)\n
uniform float     uTransparency;
\n#endif\n

\n#ifdef TRANSP_WBL\n

layout(location = 0) out vec4  accum;
layout(location = 1) out float revealage;

float
luminance(vec3 clr) {
  return dot(clr, vec3(0.212671, 0.715160, 0.072169));
}

void
applytransp(inout vec4 clr) {
\n#if defined(TRANSP_TEX) || defined(TRANSP_COLOR)\n
  vec4 c;
\n#define TRANSP_TANSPARENCY\n
\n#endif\n

\n#ifdef TRANSP_TEX\n
  c = texture(uTransparentTex, TRANSP_TEXCOORD);
\n#elif defined(TRANSP_COLOR)\n
  c = uTransparent;
\n#else\n
\n#define TRANSP_NO_COLOR\n
\n#endif\n

\n#ifndef TRANSP_NO_COLOR\n
\n#if defined(TRANSP_A_ONE)\n
  clr.a   *= c.a * uTransparency;
\n#elif defined(TRANSP_A_ZERO)\n
  clr.a   *= 1.0 - c.a * uTransparency;
\n#elif defined(TRANSP_RGB_ONE)\n
  clr.rgb *= c.rgb            * uTransparency;
  clr.a   *= luminance(c.rgb) * uTransparency;
\n#elif defined(TRANSP_RGB_ZERO)\n
  clr.rgb *= vec3(1.0) - c.rgb * uTransparency;
  clr.a   *= 1.0       - luminance(c.rgb) * uTransparency;
\n#endif\n
\n#elif defined(TRANSP_TANSPARENCY)\n
  clr.a   *= uTransparency;
\n#endif\n
}

void
transpWrite(vec4 clr /* , vec4 transmit */) {
  float a, b, w, z;

  applytransp(clr);

  /*
   clr.a *= 1.0 - clamp((transmit.r + transmit.g + transmit.b) * (1.0 / 3.0),
   0, 1);
   */

  /* Intermediate terms to be cubed */
  a = min(1.0, clr.a) * 8.0 + 0.01;
  b =-gl_FragCoord.z * 0.95 + 1.0;

  /* If your scene has a lot of content very close to the far plane,
   then include this line (one rsqrt instruction):
   b /= sqrt(1e4 * abs(csZ));
   */

  w = clamp(a * a * a * 1e8 * b * b * b, 1e-2, 3e2);

  /* alternative: equation (10)
   float dz = (1 - gl_FragCoord.z);
   w = clamp(clr.a * max(1e-2, 3e3 * dz * dz * dz), 1e-2, 3e2);
   */

  if (lightAttn == 0.0) {
    /* w = 0.0; */
    revealage = 1.0;
    accum     = vec4(0.0);
    return;
  }

  revealage = clr.a;
  accum     = vec4(clr.rgb * clr.a, clr.a) * w;
}

\n#endif\n
\n#endif\n
)
