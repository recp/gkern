/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef render_light_h
#define render_light_h

#include "../../../include/gk/gk.h"

void
gkRenderPrimForLight(GkScene     * __restrict scene,
                     GkModelInst * __restrict modelInst,
                     GkPrimitive * __restrict prim,
                     GkProgram   * __restrict prog);

void
gkRenderPrimPerLight(GkScene     * __restrict scene,
                     GkPrimitive * __restrict prim,
                     GkProgram   * __restrict prog);

void
gkRenderTranspPrimPerLight(GkScene     * __restrict scene,
                           GkPrimitive * __restrict prim,
                           GkProgram   * __restrict prog);

#endif /* render_light_h */
