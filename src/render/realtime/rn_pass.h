/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef pass_h
#define pass_h

#include "../../../include/gk/gk.h"
#include "../../../include/gk/material.h"
#include "../../../include/gk/pass.h"

void
gkRenderPass(GkScene    * __restrict scene,
             GkPrimInst * __restrict primInst,
             GkPass     * __restrict pass);

#endif /* pass_h */
