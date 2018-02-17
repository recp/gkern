/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef rn_transp_h
#define rn_transp_h

#include "../../../include/gk/gk.h"

bool
gkPrimIsTransparent(GkScene     * __restrict scene,
                    GkModelInst * __restrict modelInst,
                    GkPrimInst  * __restrict primInst);

bool
gkIsTransparent(GkScene    * __restrict scene,
                GkMaterial * __restrict mat);

#endif /* rn_transp_h */
