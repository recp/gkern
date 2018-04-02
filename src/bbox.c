/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "common.h"
#include "../include/gk/bbox.h"

#include <string.h>

void
gkTransformAABB(GkTransform * __restrict trans,
                GkBBox                   bbox[2]) {
  glm_aabb_transform(bbox[0], trans->world, bbox[1]);
}
