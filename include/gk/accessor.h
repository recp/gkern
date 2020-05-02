/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
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
