/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_color_h
#define gk_color_h
#ifdef __cplusplus
extern "C" {
#endif

typedef struct GkColorRGBA {
  float R;
  float G;
  float B;
  float A;
} GkColorRGBA;

typedef union GkColor {
  GkColorRGBA rgba;
  vec4        vec;
} GkColor;

#ifdef __cplusplus
}
#endif
#endif /* gk_color_h */
