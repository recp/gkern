/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_bbox_h
#define gk_bbox_h

#include "common.h"
#include <cglm/cglm.h>

typedef struct GkBBox {
  vec3  min;
  vec3  max;
  vec3  center;
  float radius;
} GkBBox;

#endif /* gk_bbox_h */
