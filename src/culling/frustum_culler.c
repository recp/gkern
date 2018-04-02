/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "frustum_culler.h"

#include "../types/impl_scene.h"
#include "../render/realtime/rn_transp.h"

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
  GkNodePage   *np;
  GkSceneImpl  *sceneImpl;
  GkNode       *node;
  GkModelInst  *modelInst;
  GkFrustum    *frustum;
  GkRenderList *rl[2];
  vec4         *camPlanes;
  size_t        i;

  sceneImpl = (GkSceneImpl *)scene;
  np        = sceneImpl->lastPage;
  frustum   = &cam->frustum;
  camPlanes = frustum->planes;

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
          || !(modelInst = node->model))
        continue;

      while (modelInst) {
        if (glm_aabb_frustum(modelInst->bbox, camPlanes)) {
          GkPrimInst *prims, *primInst;
          int32_t     j, primc;

          prims = modelInst->prims;
          primc = modelInst->primc;

          for (j = 0; j < primc; j++) {
            if (glm_aabb_frustum(prims[j].bbox, camPlanes)) {
              int isTransp;

              primInst = &prims[j];

              (void)gkMaterialFor(scene, modelInst, primInst);

              isTransp = gkPrimIsTransparent(scene, modelInst, primInst);
              if (rl[isTransp]->count == rl[isTransp]->size) {
                rl[isTransp]->size += 512;
                rl[isTransp] = realloc(rl[isTransp], rnListSize(rl[isTransp]));
              }

              rl[isTransp]->items[rl[isTransp]->count] = primInst;
              rl[isTransp]->count++;

              gkUpdateSceneAABB(scene, primInst->bbox);
            }
          }
        }

        modelInst = modelInst->next;
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
