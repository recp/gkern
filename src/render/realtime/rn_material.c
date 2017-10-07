/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../../include/gk/gk.h"
#include "../../../include/gk/material.h"

#include "rn_material.h"
#include "rn_pass.h"

void
gkApplyMaterial(GkScene     * __restrict scene,
                GkModelInst * __restrict modelInst,
                GkPrimitive * __restrict prim,
                GkMaterial  * __restrict material) {
  GkPass *pass;

  if (!material || !material->technique)
  return;

  if (!(pass = material->technique->pass)
      && !(material->technique->pass = pass = gkGetOrCreatPass(material)))
  return;

  while (pass) {
    GkProgInfo *pinfo;
    if ((pinfo = pass->pinfo)) {
      if (scene->currentProgram != pinfo->prog) {
        glUseProgram(pinfo->prog);
        scene->currentProgram = pinfo->prog;
      }

      gkUniformMaterial(pinfo, material);
      gkRenderPass(scene, modelInst, prim, pass);
    }

    pass = pass->next;
  }
}

void
gkApplyMaterials(GkScene     * __restrict scene,
                 GkPrimitive * __restrict prim,
                 GkModelInst * __restrict modelInst,
                 GkMaterial  * __restrict modelMaterial) {
  GkMaterial *material;

  material = NULL;

  /* instance primitive specific effects */
  if (modelInst->prims) {
    GkPrimInst *primInst;

    primInst = rb_find(modelInst->prims, prim);
    if (primInst)
      material = primInst->material;
    else
      material = prim->material;
  }

  if (!material)
    material = modelMaterial;

  gkApplyMaterial(scene,
                  modelInst,
                  prim,
                  material);
}
