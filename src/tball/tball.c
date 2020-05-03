/*
 * This file is part of the gk project (https://github.com/recp/gk)
 * Copyright (c) Recep Aslantas.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "../common.h"
#include "tball.h"
#include "mouse.h"
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

  glm_mat4_identity(tball->trans);

  return tball;
}

void
gkTrackballAttach(GkTrackball * __restrict tball,
                  GkScene     * __restrict scene,
                  GkNode      * __restrict node,
                  vec3                     pivot) {
  GkTransform *trans;

#ifdef DEBUG
  assert(tball && scene && "invalid params!");
#endif

  tball->scene    = scene;
  tball->velocity = 2.5f;
  tball->center   = pivot;

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
               GkPoint                  p,
               vec3                     vec) {
  GkScene *scene;
  mat4    m;
  vec3    c;
  float   x, y, z, d;

  scene = tball->scene;

  glm_mat4_mul(scene->camera->viewProj, tball->nodeTrans->local, m);
  glm_project(tball->center, m, scene->viewport, c);

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

  glm_vec3_normalize(vec);
}
