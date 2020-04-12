/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
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
