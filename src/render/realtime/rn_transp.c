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
gkPrimIsTransparent(GkScene     * __restrict scene,
                    GkModelInst * __restrict modelInst,
                    GkPrimInst  * __restrict primInst) {
  return primInst->activeMaterial->transparent != NULL
         && primInst->activeMaterial->transparent->amount > 0;
}

bool
gkIsTransparent(GkScene    * __restrict scene,
                GkMaterial * __restrict mat) {
  return mat->transparent != NULL && mat->transparent->amount > 0;
}
