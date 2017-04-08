/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

/* more or less red books's image structure is used here */

#ifndef gk_image_h
#define gk_image_h

#include "gk-common.h"

typedef struct GkImageMip {
  struct GkImageMip *next;
  GLvoid            *data;
  GLsizei            width;
  GLsizei            height;
  GLsizei            depth;
  GLsizeiptr         mipStride;
  GLuint             level;
} GkImageMip;

typedef struct GkImage {
  GkImageMip  mips;
  GLuint      index;  /* texture id */
  GLenum      target; /* 1D, 2D, 3D, cube... */
  GLenum      iformat;
  GLenum      format;
  GLenum      type;
  GLenum      swizzle[4];
  GLuint      lastlevel;
  GLsizei     mipLevels;
  GLsizei     slices;
  GLsizeiptr  sliceStride;
  size_t      totalsize;
} GkImage;

void
gkImageLoad(GkImage * __restrict image,
            void    * __restrict data);

#endif /* gk_image_h */
