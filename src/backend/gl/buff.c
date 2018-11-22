/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "buff.h"

static
GLenum
gk__gl_buffType(GkEnumType type);

static
GLenum
gk__gl_usage(GkEnumType type);

static
GLenum
gk__gl_buffType(GkEnumType type) {
  switch (type) {
    case GK_ARRAY:   return GL_ARRAY_BUFFER;
    case GK_INDEX:   return GL_ELEMENT_ARRAY_BUFFER;
    case GK_UNIFORM: return GL_UNIFORM_BUFFER;
    default:         return 0;
  }
}

static
GLenum
gk__gl_usage(GkEnumType type) {
  switch (type) {
    case GK_STATIC_DRAW:   return GL_STATIC_DRAW;
    case GK_STATIC_READ:   return GL_STATIC_READ;
    case GK_DYNAMIC_DRAW:  return GL_DYNAMIC_DRAW;
    case GK_DYNAMIC_READ:  return GL_DYNAMIC_READ;
    default:               return 0;
  }
}

GK_EXPORT
GkGpuBuffer*
gkGpuBufferNew(GkContext  * __restrict ctx,
               GkEnumType              type,
               size_t                  size) {
  GkGpuBuffer *gbuff;

  gbuff         = calloc(1, sizeof(*gbuff));
  gbuff->size   = (GLsizei)size;
  gbuff->usage  = GL_STATIC_DRAW;
  gbuff->target = gk__gl_buffType(type);

  glGenBuffers(1, &gbuff->vbo);

  return gbuff;
}

GK_EXPORT
void
gkGpuBufferFeed(GkGpuBuffer * __restrict gbuff,
                GkEnumType               usage,
                const void  * __restrict data) {
  gbuff->usage = gk__gl_usage(usage);

  /* TODO: get from state manager */
  glBindBuffer(gbuff->target, gbuff->vbo);
  glBufferData(gbuff->target, gbuff->size, data, gbuff->usage);
}
