/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "gk_tball.h"
#include "gk_tball_mouse.h"
#include "../../include/gk-transform.h"
#include "../default/gk_transform.h"
#include <string.h>

GkTrackball*
gk_tball_new() {
  GkTrackball *tball;

  tball = calloc(sizeof(*tball), 1);
  tball->enabled  = true;
  tball->mouse    = gk_tball_mouse;
  tball->mouse2   = gk_tball_mouse_ws;

  return tball;
}

void
gk_tball_init(GkTrackball * __restrict tball,
              GkScene     * __restrict scene,
              GkNode      * __restrict node,
              GkBBox      * __restrict bbox) {
  GkMatrix *trans;
  assert(tball && scene && bbox && "invalid params!");

  tball->scene = scene;
  tball->bbox  = bbox;

  trans = scene->trans;
  if (!trans) {
    trans = malloc(sizeof(*trans));
    trans->refc        = 1;
    trans->cmatIsValid = 0;

    glm_mat4_dup(GLM_MAT4_IDENTITY, trans->matrix);
    glm_mat4_dup(GLM_MAT4_IDENTITY, trans->cmat);
    scene->trans = trans;
  }

  if (node)
    tball->matrix = node->matrix;
  else
    tball->matrix = gk_def_idmat();
}

void
gkTrackballFree(GkTrackball *tball) {
  free(tball);
}

void
gk_tall_vec(GkTrackball * __restrict tball,
            GkPoint p,
            vec3    vec) {
  mat4    m;
  GkPoint c;
  GkRect  vrect;
  float   x, y, z, x2, y2, r;

  glm_mat4_mul(tball->scene->pv,
               tball->matrix->cmat,
               m);

  vrect = tball->scene->vrect;
  r = tball->bbox->radius;
  c = gk_project2d(vrect,
                   m,
                   tball->bbox->center);

  x = (p.x - c.x) / (vrect.size.w * 0.5f);
  y = (p.y - c.y) / (vrect.size.w * 0.5f);

  r  = r * r / 2.0f;
  x2 = x * x;
  y2 = y * y;

  if ((x2 + y2) <= r)
    z = sqrtf(2.0f * r - x2 - y2);
  else
    z = r / sqrtf(x2 + y2);

  vec[0] = x;
  vec[1] = y;
  vec[2] = z;

  glm_vec_normalize(vec);
}
