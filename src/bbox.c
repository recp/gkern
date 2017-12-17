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
                GkBBox      * __restrict bbox) {
  vec3 v[2], xa, xb, ya, yb, za, zb, t;

  glm_vec_scale(trans->world[0], bbox->local.vec.min[0], xa);
  glm_vec_scale(trans->world[0], bbox->local.vec.max[0], xb);

  glm_vec_scale(trans->world[1], bbox->local.vec.min[1], ya);
  glm_vec_scale(trans->world[1], bbox->local.vec.max[1], yb);

  glm_vec_scale(trans->world[2], bbox->local.vec.min[2], za);
  glm_vec_scale(trans->world[2], bbox->local.vec.max[2], zb);

  /* vec3.min(xa, xb) + vec3.min(ya, yb) + vec3.min(za, zb) + translation */
  glm_vec_minv(xa, xb, v[0]);
  glm_vec_minv(ya, yb, t);
  glm_vec_add(v[0], t, v[0]);
  glm_vec_minv(za, zb, t);
  glm_vec_add(v[0], t, v[0]);
  glm_vec_add(v[0], trans->world[3], v[0]);

  /* vec3.max(xa, xb) + vec3.max(ya, yb) + vec3.max(za, zb) + translation */
  glm_vec_maxv(xa, xb, v[1]);
  glm_vec_maxv(ya, yb, t);
  glm_vec_add(v[1], t, v[1]);
  glm_vec_maxv(za, zb, t);
  glm_vec_add(v[1], t, v[1]);
  glm_vec_add(v[1], trans->world[3], v[1]);

  memcpy(bbox->world.arr, v, sizeof(v));
}
