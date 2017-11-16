/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../../include/gk/gk.h"
#include "../../../include/gk/material.h"

#include "../../gpu_state/common.h"
#include "../../default/def_effect.h"

#include "rn_material.h"
#include "rn_pass.h"

void
gkPrepMaterial(GkScene     *scene,
               GkModelInst *modelInst) {
  GkModel     *model;
  GkPrimitive *primi;
  GkMaterial  *modelMaterial;
  
  model = modelInst->model;
  /*
   make sure model instance has a material
   (to avoid access model multiple times)
   */
  modelMaterial = NULL;
  if (!modelInst->material) {
    /* model doesn't has a material set default material */
    if (!(modelInst->activeMaterial = modelMaterial = model->material))
      modelInst->activeMaterial = modelMaterial = gk_def_material();
  }
  
  /* make sure every primitive has a material */
  primi = model->prim;
  while (primi) {
    if (!(primi->activeMaterial = primi->material))
      primi->activeMaterial = modelMaterial;
    primi = primi->next;
  }
}

void
gkApplyMaterial(GkScene     * __restrict scene,
                GkModelInst * __restrict modelInst,
                GkPrimitive * __restrict prim,
                GkMaterial  * __restrict material) {
  GkPass *pass;

  if (!material || !material->technique)
    return;

  if (scene->_priv.overridePass) {
    pass = scene->_priv.overridePass;
    goto apply;
  }
  
  if (!(pass = material->technique->pass)
      && !(material->technique->pass =
           pass = gkGetOrCreatPass(scene, prim, material)))
    return;

apply:
  while (pass) {
    gkRenderPass(scene, modelInst, prim, material, pass);
    pass = pass->next;
  }
}

void
gkApplyMaterials(GkScene     * __restrict scene,
                 GkModelInst * __restrict modelInst,
                 GkPrimitive * __restrict prim) {
  GkMaterial *material;

  if (scene->_priv.overrideMaterial) {
    material = scene->_priv.overrideMaterial;
    goto apply;
  }

  material = NULL;

  /* instance primitive specific effects */
  if (modelInst->prims) {
    GkPrimInst *primInst;
    if ((primInst = rb_find(modelInst->prims, prim)))
      material = primInst->material;
  }

  if (!material)
    material = prim->activeMaterial;

  if (!material)
    material = gk_def_material();

apply:
  gkApplyMaterial(scene, modelInst, prim, material);
}
