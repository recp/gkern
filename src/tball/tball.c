/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "tball.h"
#include "tball_mouse.h"
#include "../../include/gk/transform.h"
#include "../default/transform.h"
#include <string.h>

GkTrackball*
gk_tball_new() {
  GkTrackball *tball;

  tball = calloc(1, sizeof(*tball));
  tball->enabled  = true;
  tball->mouse    = gk_tball_mouse;
  tball->mouse2   = gk_tball_mouse_ws;

  return tball;
}

void
gk_tball_attach(GkTrackball * __restrict tball,
                GkScene     * __restrict scene,
                GkNode      * __restrict node,
                GkBBox      * __restrict bbox) {
  GkTransform *trans;
  assert(tball && scene && bbox && "invalid params!");

  tball->scene    = scene;
  tball->bbox     = bbox;
  tball->velocity = 2.5f;

  trans = scene->trans;
  if (!trans) {
    trans = gkAllocTransform(scene);

    glm_mat4_copy(GLM_MAT4_IDENTITY, trans->local);
    glm_mat4_copy(GLM_MAT4_IDENTITY, trans->world);
    scene->trans = trans;
  }

  if (node)
    tball->nodeTrans = node->trans;
  else
    tball->nodeTrans = gk_def_idmat();

  tball->node = node;
}

void
gk_tball_free(GkTrackball *tball) {
  free(tball);
}

void
gk_tall_vec(GkTrackball * __restrict tball,
            GkPoint p,
            vec3    vec) {
  mat4    m;
  vec3    center;
  GkPoint c;
  GkRect  vrc;
  float   x, y, z, d;

  glm_mat4_mul(tball->scene->camera->viewProj,
               tball->nodeTrans->local,
               m);

  glm_vec_center(tball->bboxMin, tball->bboxMax, center);

  vrc = tball->scene->vrect;
  c   = gk_project2d(vrc, m, center);

  x = (p.x - c.x) / vrc.size.w;
  y = (p.y - c.y) / vrc.size.h;
  d = x * x + y * y;

  if (d <= 0.5f)
    z = sqrtf(1.0f - d);
  else
    z = 0.5f / sqrtf(d);

  vec[0] = x;
  vec[1] = y;
  vec[2] = z;

  glm_vec_normalize(vec);
}
