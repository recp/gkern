/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "common.h"
#include "../include/gk/bbox.h"
#include "../include/gk/scene.h"

#include <string.h>

void
gkTransformAABB(GkTransform * __restrict trans,
                GkBBox                   bbox[2]) {
  glm_aabb_transform(bbox[0], trans->world, bbox[1]);
}

GK_EXPORT
bool
gkSceneIs2D(GkScene *scene) {
  return glm_eq(scene->bbox[1][0] - scene->bbox[0][0], 0.0f)
         || glm_eq(scene->bbox[1][1] - scene->bbox[0][1], 0.0f)
         || glm_eq(scene->bbox[1][2] - scene->bbox[0][2], 0.0f);
}
