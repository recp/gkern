/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../../include/gk/gk.h"
#include "../../../include/gk/material.h"

#include "../../gpu_state/common.h"

#include "rn_material.h"
#include "rn_pass.h"

void
gkApplyMaterial(GkScene     * __restrict scene,
                GkModelInst * __restrict modelInst,
                GkPrimitive * __restrict prim,
                GkMaterial  * __restrict material) {
  GkContext *ctx;
  GkPass    *pass;

  if (!material || !material->technique)
    return;

  if (!(pass = material->technique->pass)
      && !(material->technique->pass =
           pass = gkGetOrCreatPass(scene, prim, material)))
    return;

  ctx = scene->_priv.ctx;
  while (pass) {
    GkProgInfo *pinfo;
    if ((pinfo = pass->pinfo)) {
      if (ctx->currState->pinfo != pinfo)
        gkUseProgram(ctx, pinfo);

      if (pinfo->lastMaterial != material)
        gkUniformMaterial(pinfo, material);

      gkRenderPass(scene,
                   modelInst,
                   prim,
                   material,
                   pass);
    }

    pass = pass->next;
  }
}

void
gkApplyMaterials(GkScene     * __restrict scene,
                 GkModelInst * __restrict modelInst,
                 GkPrimitive * __restrict prim) {
  GkMaterial *material;

  material = NULL;

  /* instance primitive specific effects */
  if (modelInst->prims) {
    GkPrimInst *primInst;
    if ((primInst = rb_find(modelInst->prims, prim)))
      material = primInst->material;
  }

  if (!material)
    material = prim->activeMaterial;

  gkApplyMaterial(scene,
                  modelInst,
                  prim,
                  material);
}
