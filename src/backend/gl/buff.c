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
