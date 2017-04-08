/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_bbox_h
#define gk_bbox_h

#include "gk-common.h"

typedef struct GkBBox {
  float min[3];
  float max[3];
  float center[3];
  float radius;
  bool  isvalid;
} GkBBox;

#endif /* gk_bbox_h */
