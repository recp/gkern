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

  box = bbox->world.arr;
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

  flist_sp_destroy(&cam->frustum.objs);
  cam->frustum.objsCount = 0;

  while (np) {
    for (i = 0; i < gk_nodesPerPage; i++) {
      node = &np->nodes[i];

      /* unallocated node */
      if (!(node->flags & GK_NODEF_NODE)
          || !(modelInst = node->model))
        continue;

      while (modelInst) {
        if (!modelInst->bbox)
          goto cont;

        if (gkAABBInFrustum(modelInst->bbox, cam->frustum.planes)) {
          flist_sp_insert(&cam->frustum.objs, modelInst);
          cam->frustum.objsCount++;
        }

      cont:
        modelInst = modelInst->next;
      }
    }

    np = np->next;
  }
}
