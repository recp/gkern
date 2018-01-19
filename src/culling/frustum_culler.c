/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "frustum_culler.h"
#include <math.h>
#include <string.h>
#include <ds/forward-list-sep.h>
#include "../types/impl_scene.h"

/*
 Cull AAB ref:
   http://www.txutxi.com/?p=584
   http://old.cescg.org/CESCG-2002/DSykoraJJelinek/
 */

extern uint32_t gk_nodesPerPage;

GK_INLINE
bool
gkAABBInFrustum(GkBBox *bbox, GkPlane planes[6]) {
  vec3  *box;
  float *p, dp;
  int    i;

  box = bbox->world;
  for (i = 0; i < 6; i++) {
    p  = planes[i];
    dp = p[0] * box[p[0] > 0.0f][0]
       + p[1] * box[p[1] > 0.0f][1]
       + p[2] * box[p[2] > 0.0f][2];

    if (dp < -p[3])
      return false;
  }

  return true;
}

bool
gkPrimIsInFrustum(GkScene     * __restrict scene,
                  GkCamera    * __restrict cam,
                  GkPrimitive * __restrict prim) {
  return gkAABBInFrustum(prim->bbox, cam->frustum.planes);
}

GK_EXPORT
void
gkCullFrustum(GkScene  * __restrict scene,
              GkCamera * __restrict cam) {
  GkNodePage  *np;
  GkSceneImpl *sceneImpl;
  GkNode      *node;
  GkModelInst *modelInst;
  size_t       i;

  sceneImpl = (GkSceneImpl *)scene;
  np        = sceneImpl->lastPage;

  if (cam->frustum.objs)
    free(cam->frustum.objs);

  cam->frustum.objsCount = 0;
  cam->frustum.objsLen   = 1024;
  cam->frustum.objs      = malloc(sizeof(void *) * cam->frustum.objsLen);

  while (np) {
    for (i = 0; i < gk_nodesPerPage; i++) {
      node = &np->nodes[i];

      /* unallocated node */
      if (!(node->flags & GK_NODEF_NODE)
          || !(modelInst = node->model))
        continue;

      while (modelInst) {
        if (modelInst->bbox
            && glm_aabb_frustum(modelInst->bbox->world,
                                cam->frustum.planes)) {
          if (cam->frustum.objsCount == cam->frustum.objsLen) {
            cam->frustum.objsLen += 512;
            cam->frustum.objs = realloc(cam->frustum.objs,
                                        sizeof(void *) * cam->frustum.objsLen);
          }

          cam->frustum.objs[cam->frustum.objsCount] = modelInst;
          cam->frustum.objsCount++;
        }

        modelInst = modelInst->next;
      }
    }

    np = np->next;
  }
}

GK_EXPORT
void
gkCullSubFrustum(GkFrustum * __restrict frustum,
                 GkFrustum * __restrict subfrustum) {
  GkModelInst **it, *modelInst;
  size_t        i;

  if (subfrustum->objs)
    free(subfrustum->objs);

  subfrustum->objsCount = 0;
  subfrustum->objsLen   = glm_min(1024, subfrustum->objsCount);
  subfrustum->objs      = malloc(sizeof(void *) * subfrustum->objsLen);

  it = frustum->objs;
  for (i = 0; i < frustum->objsCount; i++) {
    modelInst = it[i];

    if (glm_aabb_frustum(modelInst->bbox->world,
                         subfrustum->planes)) {
      if (subfrustum->objsCount == subfrustum->objsLen) {
        subfrustum->objsLen += 512;
        subfrustum->objs = realloc(subfrustum->objs,
                                   sizeof(void *) * subfrustum->objsLen);
      }

      subfrustum->objs[subfrustum->objsCount] = modelInst;
      subfrustum->objsCount++;
    }
  }
}

GK_EXPORT
void
gkBoxInFrustum(GkFrustum * __restrict frustum,
               vec3                   box[2]) {
  GkModelInst **it;
  vec3          t[2];
  size_t        i, c;

  glm_vec_broadcast(FLT_MAX,  t[0]);
  glm_vec_broadcast(-FLT_MAX, t[1]);

  it = frustum->objs;
  c  = frustum->objsCount;

  for (i = 0; i < c; i++)
    glm_aabb_merge(t, it[i]->bbox->world, t);

  memcpy(box, t, sizeof(t));
}
