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

struct GkTransform;

struct GkBBoxVec {
  vec3  min;
  vec3  max;
};

union GkBBoxU {
  vec3             arr[2];
  struct GkBBoxVec vec;
};

typedef struct GkBBox {
  union GkBBoxU local;
  union GkBBoxU world;
} GkBBox;

#define boxMin world.vec.min
#define boxMax world.vec.max

#define bboxMin bbox->world.vec.min
#define bboxMax bbox->world.vec.max

void
gkTransformAABB(struct GkTransform * __restrict trans,
                GkBBox             * __restrict bbox);

#endif /* gk_bbox_h */
