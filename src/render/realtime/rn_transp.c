/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../../include/gk/gk.h"

#include "rn_light.h"
#include "rn_pass.h"
#include "rn_transp.h"

bool
gkPrimIsTransparent(GkModelInst * __restrict modelInst,
                    GkMaterial  * __restrict modelMaterial,
                    GkPrimitive * __restrict prim) {
  GkMaterial *material;
  
  material = NULL;

  /* model's material */
  if (!modelMaterial) {
    if (!modelInst->prims
        || modelInst->material
        || modelInst->model->material) {
      if (!(modelMaterial = modelInst->material))
        modelMaterial = modelInst->model->material;
    }
  }
  
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
  
  if (!material)
    return false;

  return material->transparent != NULL;
}
