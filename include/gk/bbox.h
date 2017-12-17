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

struct GkBBoxVec {
  vec3  min;
  vec3  max;
};

union GkBBoxU {
  vec3             arr[2];
  struct GkBBoxVec vec;
};

typedef struct GkBBox {
  union GkBBoxU box;
} GkBBox;

#define boxMin box.vec.min
#define boxMax box.vec.max

#define bboxMin bbox->box.vec.min
#define bboxMax bbox->box.vec.max

#endif /* gk_bbox_h */
