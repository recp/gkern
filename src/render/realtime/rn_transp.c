/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../common.h"
#include "../../../include/gk/gk.h"

#include "rn_light.h"
#include "rn_pass.h"
#include "rn_transp.h"

bool
gkPrimIsTransparent(GkModelInst * __restrict modelInst,
                    GkPrimitive * __restrict prim) {
  GkMaterial *material;
  
  material = NULL;

  /* instance primitive specific effects */
  if (modelInst->prims) {
    GkPrimInst *primInst;
    primInst = rb_find(modelInst->prims, prim);
    if (primInst)
      material = primInst->material;
  }
  
  if (!material && !(material = prim->activeMaterial))
    return false;

  return material->transparent != NULL;
}
