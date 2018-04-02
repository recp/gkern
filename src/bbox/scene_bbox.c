/*
 * Copyright (c), Recep Aslantas.
 * Full license can be found in the LICENSE file
 */

#include "scene_bbox.h"

GK_EXPORT
void
gkUpdateSceneAABB(GkScene * __restrict scene, GkBBox bbox) {
  glm_aabb_merge(scene->bbox, bbox, scene->bbox);
}
