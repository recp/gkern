/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../../include/gk/gk.h"
#include "../../../include/gk/material.h"

#include "rn_light.h"
#include "rn_material.h"
#include "rn_pass.h"

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
gkRenderPass(GkScene     * __restrict scene,
             GkModelInst * __restrict modelInst,
             GkPrimitive * __restrict prim,
             GkPass      * __restrict pass) {
  GkProgInfo *pinfo;

  if (!pass || !prim || !scene)
  return;

  if (!(pinfo = pass->pinfo))
  return;

  gkUniformTransform(pinfo, modelInst->trans);
  gkRenderPrimPerLight(scene, prim, pinfo);
}
