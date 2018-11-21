/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef buffer_h
#define buffer_h

#include "common.h"

typedef enum GkEnumType {
  GK_NONE         = 0,
  GK_ARRAY        = 1,
  GK_INDEX        = 2,
  GK_UNIFORM      = 3,

  GK_STATIC_DRAW  = 4,
  GK_STATIC_READ  = 5,
  GK_DYNAMIC_DRAW = 6,
  GK_DYNAMIC_RED  = 7
} GkEnumType;

typedef struct GkGPUBuffer {
  struct GkGPUBuffer *prev;
  struct GkGPUBuffer *next;
  GLuint              vbo;
  GLsizei             size;
  GLenum              usage;
  GLenum              target;
} GkGpuBuffer;

typedef struct GkBuffer {
  void  *data;
  size_t len;
  size_t count;
  size_t stride;
} GkBuffer;


GK_EXPORT
GkGpuBuffer*
gkGpuBufferNew(struct GkContext * __restrict ctx,
               GkEnumType                    type,
               size_t                        size);

GK_EXPORT
void
gkGpuBufferFeed(GkGpuBuffer * __restrict gbuff,
                GLenum                   usage,
                const void  * __restrict data);

#endif /* buffer_h */
