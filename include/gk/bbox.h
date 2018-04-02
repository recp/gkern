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

typedef  vec3  GkBBox[2];

void
gkTransformAABB(struct GkTransform * __restrict trans,
                GkBBox                          bbox[2]);

#endif /* gk_bbox_h */
