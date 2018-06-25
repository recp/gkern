/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef buffer_h
#define buffer_h

#include "common.h"

typedef struct GkGPUBuffer {
  struct GkGPUBuffer *prev;
  struct GkGPUBuffer *next;
  GLuint              vbo;
  GLsizei             size;
  GLenum              usage;
  GLenum              type;
  GLenum              target;
} GkGPUBuffer;

#endif /* buffer_h */
