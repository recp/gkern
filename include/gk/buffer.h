/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef buffer_h
#define buffer_h

#include "common.h"

typedef struct GkBuffer {
  struct GkBuffer *prev;
  struct GkBuffer *next;

  GLuint           vbo;
  GLsizei          size;
  GLenum           usage;
  GLenum           type;
  GLenum           target;
} GkBuffer;

#endif /* buffer_h */
