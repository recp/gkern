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
gkTrackballAlloc() {
  GkTrackball *tball;

  tball = calloc(1, sizeof(*tball));
  tball->enabled  = true;
  tball->mouse    = gkTrackballMouse;
  tball->mouse2   = gkTrackballMouseWs;

  return tball;
}

void
gkTrackballAttach(GkTrackball * __restrict tball,
                  GkScene     * __restrict scene,
                  GkNode      * __restrict node,
                  GkBBox                   bbox) {
  GkTransform *trans;

  assert(tball && scene && bbox && "invalid params!");

  tball->scene    = scene;
  tball->bbox     = bbox;
  tball->velocity = 2.5f;

  trans = scene->trans;
  if (!trans) {
    trans = gkAllocTransform(scene);
    scene->trans = trans;
  }

  if (node)
    tball->nodeTrans = node->trans;
  else
    tball->nodeTrans = gk_def_idmat();

  tball->node = node;
}

void
gkTrackballFree(GkTrackball *tball) {
  free(tball);
}

void
gkTrackballVec(GkTrackball * __restrict tball,
            GkPoint p,
            vec3    vec) {
  GkScene *scene;
  mat4    m;
  vec3    center, c;
  float   x, y, z, d;

  scene = tball->scene;

  glm_mat4_mul(scene->camera->viewProj, tball->nodeTrans->local, m);
  glm_vec_center(tball->bbox[0], tball->bbox[1], center);

  glm_project(center, m, scene->viewport, c);

  x = (p.x - c[0]) / scene->viewport[2];
  y = (p.y - c[1]) / scene->viewport[3];
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
