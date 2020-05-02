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

#ifndef gk_accessor_h
#define gk_accessor_h

#include "common.h"

typedef struct GkGPUAccessor {
  struct GkGPUBuffer *buffer;

  void               *min;
  void               *max;

  uint32_t            itemCount;
  uint32_t            itemSize;
  GkType              itemType;

  uint32_t            byteOffset;
  uint32_t            byteStride;
  uint32_t            count;

  uint32_t            gpuTarget;
  bool                normalized;
} GkGPUAccessor;

#endif /* gk_accessor_h */
