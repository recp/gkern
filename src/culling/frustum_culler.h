/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef frustum_culler_h
#define frustum_culler_h

#include "../../include/gk/gk.h"

bool
gkPrimIsInFrustum(GkScene     * __restrict scene,
                  GkCamera    * __restrict cam,
                  GkPrimitive * __restrict prim);

#endif /* frustum_culler_h */
