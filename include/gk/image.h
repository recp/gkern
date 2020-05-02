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
