/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk.h"
#include "../include/gk-geom-types.h"
#include "../include/gk-transform.h"

GkPoint
gk_project2d(GkRect rect, mat4 mvp, vec3 v) {
  vec4    pos4;
  GkPoint p;

  glm_vec_dup(v, pos4);
  pos4[3] = 1.0f;

  glm_mat4_mulv(mvp, pos4, pos4);
  glm_vec4_scale(pos4, 1.0f / pos4[3], pos4); /* pos = pos / pos.w */

  p.x = rect.origin.x + rect.size.w * (pos4[0] + 1.0f) * 0.5f;
  p.y = rect.origin.y + rect.size.h * (pos4[1] + 1.0f) * 0.5f;
  return p;
}
