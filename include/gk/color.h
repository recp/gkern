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

#ifndef gk_color_h
#define gk_color_h
#ifdef __cplusplus
extern "C" {
#endif

#include <cglm/cglm.h>

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
