/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */


#include "rn_light.h"
#include "rn_material.h"
#include "rn_pass.h"

void
gkRenderPass(GkScene     * __restrict scene,
             GkModelInst * __restrict modelInst,
             GkPrimitive * __restrict prim,
             GkMaterial  * __restrict material,
             GkPass      * __restrict pass) {
  GkProgram *prog;

  if (!pass || !prim || !scene)
    return;

  if (!(prog = pass->prog))
    return;

  gkUniformTransform(pinfo, modelInst->trans);

  if (!material->transparent) {
    gkRenderPrimPerLight(scene, prim, pinfo);
  } else {
    gkRenderTranspPrimPerLight(scene, prim, pinfo);
  }
}
