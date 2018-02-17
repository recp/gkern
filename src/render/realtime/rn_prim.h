/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef rn_prim_h
#define rn_prim_h

#include "../../../include/gk/gk.h"

void
gkRenderPrim(GkScene     * __restrict scene,
             GkPrimitive * __restrict prim);

void
gkRenderPrimInst(GkScene    * __restrict scene,
                 GkPrimInst * __restrict primInst);

GK_INLINE
void
gkRenderPrims(GkScene      * __restrict scene,
              GkRenderList * __restrict rnlist) {
  GkSceneImpl *sceneImpl;
  GkPrimInst **prims;
  size_t       i, primc;

  sceneImpl = (GkSceneImpl *)scene;
  primc     = rnlist->count;
  prims     = rnlist->items;

  for (i = 0; i < primc; i++) {
    if (!scene->renderPrimFunc)
      gkRenderPrimInst(scene, prims[i]);
    else
      scene->renderPrimFunc(scene, prims[i]);
  }
}

#endif /* rn_prim_h */
