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
  GK_DYNAMIC_READ = 7
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

typedef struct GkAccessor {
  GkBuffer *buffer;

  void     *min;
  void     *max;

  uint32_t  itemCount;
  uint32_t  itemSize;
  uint32_t  itemType;

  uint32_t  byteOffset;
  uint32_t  byteStride;
  uint32_t  count;
  
  uint32_t  gpuTarget;
  bool      normalized;
} GkAccessor;

GK_EXPORT
GkGpuBuffer*
gkGpuBufferNew(struct GkContext * __restrict ctx,
               GkEnumType                    type,
               size_t                        size);

GK_EXPORT
void
gkGpuBufferFeed(GkGpuBuffer * __restrict gbuff,
                GkEnumType               usage,
                const void  * __restrict data);

#endif /* buffer_h */
