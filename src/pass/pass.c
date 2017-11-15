/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../include/gk/model.h"
#include "../../include/gk/pass.h"
#include "../../include/gk/shader.h"
#include "../../include/gk/material.h"
#include "../../include/gk/program.h"

#include "../shader/cmn_material.h"

#include <ds/rb.h>

GK_EXPORT
GkPass*
gkGetOrCreatPass(GkScene     *scene,
                 GkPrimitive *prim,
                 GkMaterial  *mat) {
  GkPass    *pass;
  GkProgram *prog;

  if ((prog = gkGetOrCreatProgForCmnMat(scene, prim, mat))) {
    pass = calloc(sizeof(*pass), 1);
    pass->prog = prog;
    return pass;
  }

  /* TODO: custom shaders */
  /*
   if (!prog) {
   }
   */

  return NULL;
}
