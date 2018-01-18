/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef src_shad_render_h
#define src_shad_render_h

#include "../../include/gk/gk.h"
#include "../../include/gk/shadows.h"
#include "../../include/gk/pass.h"

void
gkRenderShadowMap(GkScene     * __restrict scene,
                  GkModelInst * __restrict modelInst,
                  GkProgram   * __restrict prog,
                  int                      split);

#endif /* src_shad_render_h */

