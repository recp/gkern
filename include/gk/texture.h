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

#ifndef gk_texture_h
#define gk_texture_h

#include "common.h"
#include "image.h"
#include "color.h"
#include <stdbool.h>

struct GkBindTexture;

typedef enum GkWrapMode {
  GK_WRAP_MODE_WRAP        = 0,
  GK_WRAP_MODE_MIRROR      = 1,
  GK_WRAP_MODE_CLAMP       = 2,
  GK_WRAP_MODE_BORDER      = 3,
  GK_WRAP_MODE_MIRROR_ONCE = 4
} GkWrapMode;

typedef enum GkMinFilter {
  GK_MINFILTER_LINEAR      = 0,
  GK_MINFILTER_NEAREST     = 1,
  GK_MINFILTER_ANISOTROPIC = 2
} GkMinFilter;

typedef enum GkMagFilter {
  GK_MAGFILTER_LINEAR  = 0,
  GK_MAGFILTER_NEAREST = 1
} GkMagFilter;

typedef enum GkMipFilter {
  GK_MIPFILTER_LINEAR  = 0,
  GK_MIPFILTER_NONE    = 1,
  GK_MIPFILTER_NEAREST = 2
} GkMipFilter;

typedef struct GkSampler {
  const char           *uniformName;
  struct GkBindTexture *bindTexture;
  GkColor              *borderColor;
  GkWrapMode            wrapS;
  GkWrapMode            wrapT;
  GkWrapMode            wrapP;
      
  GkMinFilter           minfilter;
  GkMagFilter           magfilter;
  GkMipFilter           mipfilter;
      
  unsigned long         maxAnisotropy;
  unsigned long         mipMaxLevel;
  unsigned long         mipMinLevel;
  float                 mipBias;
} GkSampler;

typedef struct GkTexture {
  struct GkTexture *next;
  GkImage          *image;
  GkSampler        *sampler;
  GLuint            index;
  GLenum            boundUnit;
  GLenum            target; /* 1D, 2D, 3D, cube... */
  GLenum            internalFormat;
  GLenum            swizzle[4];
} GkTexture;

void
gkTexLoad(GkTexture * __restrict tex,
          bool                   ismutable);

#endif /* gk_texture_h */
