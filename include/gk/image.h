/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

/* more or less red books's image structure is used here */

#ifndef gk_image_h
#define gk_image_h

#include "common.h"

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
  GLenum      format; /* RGB, RGBA...                 */
  GLenum      type;   /* GL_UNSIGNED_BYTE, GL_FLOAT...*/
  GLsizei     mipLevels;
  GLsizei     slices;
  GLsizeiptr  sliceStride;
  size_t      totalsize;
} GkImage;

#endif /* gk_image_h */
