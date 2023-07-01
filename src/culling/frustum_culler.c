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

#include "frustum_culler.h"

#include "../types/impl_scene.h"
#include "../render/realtime/transp.h"

#include "../bbox/scene_bbox.h"

#include <math.h>
#include <string.h>
#include <ds/forward-list-sep.h>

/*
 Cull AAB ref:
   http://www.txutxi.com/?p=584
   http://old.cescg.org/CESCG-2002/DSykoraJJelinek/
 */

extern uint32_t gk_nodesPerPage;

#define rnListSizeInit(x) (sizeof(*x) + sizeof(void *) * 1024)
#define rnListSize(x)     (sizeof(*x) + sizeof(void *) * x->size)

bool
gkPrimIsInFrustum(GkScene     * __restrict scene,
                  GkCamera    * __restrict cam,
                  GkPrimitive * __restrict prim) {
  return glm_aabb_frustum(prim->bbox, cam->frustum.planes);
}

GK_EXPORT
void
gkCullFrustum(GkScene  * __restrict scene,
              GkCamera * __restrict cam) {
  GkNodePage      *np;
  GkSceneImpl     *sceneImpl;
  GkNode          *node;
  GkGeometryInst  *geomInst;
  GkFrustum       *frustum;
  GkRenderList    *rl[2];
  vec4            *camPlanes;
  size_t           i;

  sceneImpl = (GkSceneImpl *)scene;
  np        = sceneImpl->lastPage;
  frustum   = &cam->frustum;
  camPlanes = frustum->planes;

  flist_sp_destroy(&frustum->modelInsList);
  frustum->modelInsList = NULL;

  if (frustum->opaque)
    frustum->opaque->count = 0;
  else {
    frustum->opaque        = malloc(rnListSizeInit(frustum->opaque));
    frustum->opaque->count = 0;
    frustum->opaque->size  = 1024;
  }

  if (frustum->transp)
    frustum->transp->count = 0;
  else {
    frustum->transp        = malloc(rnListSizeInit(frustum->transp));
    frustum->transp->count = 0;
    frustum->transp->size  = 1024;
  }

  rl[0] = frustum->opaque;
  rl[1] = frustum->transp;

  while (np) {
    for (i = 0; i < np->count; i++) {
      node = &np->nodes[i];

      /* unallocated node */
      if (!(node->flags & GK_NODEF_NODE)
          || !(geomInst = node->geom))
        continue;

      while (geomInst) {
        if (glm_aabb_frustum(geomInst->bbox, camPlanes)) {
          GkPrimInst *prims, *primInst;
          int32_t     j, primc;

          prims = geomInst->prims;
          primc = geomInst->primc;

          for (j = 0; j < primc; j++) {
            if (glm_aabb_frustum(prims[j].bbox, camPlanes)) {
              int isTransp;

              primInst = &prims[j];

              (void)gkMaterialFor(scene, geomInst, primInst);

              isTransp = gkPrimIsTransparent(scene, geomInst, primInst);
              if (rl[isTransp]->count == rl[isTransp]->size) {
                rl[isTransp]->size += 512;
                rl[isTransp] = realloc(rl[isTransp], rnListSize(rl[isTransp]));
              }

              rl[isTransp]->items[rl[isTransp]->count] = primInst;
              rl[isTransp]->count++;

              gkUpdateSceneAABB(scene, primInst->bbox);
            }
          } /* for each prim */
          
          flist_sp_insert(&frustum->modelInsList, geomInst);
        }

        geomInst = geomInst->next;
      }
    }

    np = np->next;
  }

  /* because of realloc */
  frustum->opaque = rl[0];
  frustum->transp = rl[1];
}

GK_EXPORT
void
gkCullSubFrustum(GkFrustum * __restrict frustum,
                 GkFrustum * __restrict subfrustum) {
  GkPrimInst  **it;
  GkRenderList *rl[2], *subrl[2];
  size_t        i, j, c;

  if (subfrustum->opaque)
    subfrustum->opaque->count = 0;
  else {
    subfrustum->opaque        = malloc(rnListSizeInit(subfrustum->opaque));
    subfrustum->opaque->count = 0;
    subfrustum->opaque->size  = 1024;
  }

  if (subfrustum->transp)
    subfrustum->transp->count = 0;
  else {
    subfrustum->transp        = malloc(rnListSizeInit(subfrustum->transp));
    subfrustum->transp->count = 0;
    subfrustum->transp->size  = 1024;
  }

  rl[0]    = frustum->opaque;
  rl[1]    = frustum->transp;
  subrl[0] = subfrustum->opaque;
  subrl[1] = subfrustum->transp;

  for (i = 0; i < 2; i++) {
    c  = rl[i]->count;
    it = rl[i]->items;

    for (j = 0; j < c; j++) {
      if (glm_aabb_frustum(it[j]->bbox, subfrustum->planes)) {
        if (subrl[i]->count == subrl[i]->size) {
          subrl[i]->size += 512;
          subrl[i] = realloc(subrl[i], rnListSize(subrl[i]));
        }

        subrl[i]->items[subrl[i]->count] = it[j];
        subrl[i]->count++;
      }
    }
  }

  /* because of realloc */
  subfrustum->opaque = subrl[0];
  subfrustum->transp = subrl[1];
}

GK_EXPORT
void
gkBoxInFrustum(GkFrustum * __restrict frustum,
               vec3                   box[2]) {
  GkPrimInst  **it;
  GkRenderList *rl[2];
  vec3          t[2];
  size_t        i, j, c;

  glm_aabb_invalidate(t);

  rl[0] = frustum->opaque;
  rl[1] = frustum->transp;

  for (i = 0; i < 2; i++) {
    c  = rl[i]->count;
    it = rl[i]->items;
    for (j = 0; j < c; j++)
      glm_aabb_merge(t, it[j]->bbox, t);
  }

  memcpy(box, t, sizeof(t));
}
