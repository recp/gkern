/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../common.h"
#include "../../../include/gk/gk.h"
#include "../../../include/gk/material.h"

#include "../../state/gpu.h"
#include "../../default/def_effect.h"

#include "rn_material.h"
#include "rn_pass.h"
#include "rn_light.h"
#include "rn_prim.h"

GkMaterial*
gkMaterialFor(GkScene     *scene,
              GkModelInst *modelInst,
              GkPrimInst  *primInst) {
  GkModel     *model;
  GkSceneImpl *sceneImpl;
  GkMaterial  *material;

  material  = NULL;
  model     = modelInst->model;
  sceneImpl = (GkSceneImpl *)scene;

  if (sceneImpl->overrideMaterial)
    return sceneImpl->overrideMaterial;

  if (!(material = primInst->material))
    material = primInst->prim->material;

  if (!material && !(material = modelInst->material))
    material = model->material;

  if (!material)
    material = gk_def_material();
  
  primInst->activeMaterial = material;

  return material;
}

void
gkApplyMaterial(GkScene    * __restrict scene,
                GkPrimInst * __restrict primInst) {
  GkSceneImpl *sceneImpl;
  GkPass      *pass;
  GkMaterial  *material;
  GkPrimitive *prim;
  
  sceneImpl = (GkSceneImpl *)scene;
  if (sceneImpl->overridePass) {
    pass = sceneImpl->overridePass;
    goto apply;
  }
  
  material = primInst->activeMaterial;
  prim     = primInst->prim;

  if (!(pass = material->technique->pass)
      && !(material->technique->pass =
           pass = gkGetOrCreatPass(scene,
                                   sceneImpl->forLight,
                                   prim,
                                   material)))
    return;
  
apply:
  while (pass) {
    gkRenderPass(scene, primInst, pass);
    pass = pass->next;
  }
}
