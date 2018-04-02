/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../common.h"
#include "rn_prim.h"
#include "rn_material.h"
#include "../../../include/gk/prims/cube.h"

void
gkRenderPrim(GkScene     * __restrict scene,
             GkPrimitive * __restrict prim) {
  if (prim->flags & GK_DRAW_ELEMENTS)
    glDrawElements(prim->mode,
                   prim->count,
                   GL_UNSIGNED_INT, /* TODO: ? */
                   NULL);
  else if (prim->flags & GK_DRAW_ARRAYS)
    glDrawArrays(prim->mode, 0, prim->count);
}

void
gkRenderPrimInst(GkScene    * __restrict scene,
                 GkPrimInst * __restrict primInst) {
  GkPrimitive *prim;
  
  prim = primInst->prim;
  
  glBindVertexArray(prim->vao);
  gkApplyMaterial(scene, primInst);

  if ((scene->flags & GK_SCENEF_DRAW_PRIM_BBOX))
    gkDrawBBox(scene,
               primInst->bbox,
               primInst->trans->world);
}
