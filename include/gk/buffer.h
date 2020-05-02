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

#ifndef buffer_h
#define buffer_h

#include "common.h"
#include "type.h"

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
