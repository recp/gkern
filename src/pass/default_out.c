/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "../../include/gk/gk.h"
#include "../../include/gk/pass.h"
#include "../../include/gk/scene.h"

GkOutput gk_def_renderout = {
  .fbo        = 0,
  .depth      = 0,
  .stencil    = 0,
  .color      = NULL,
  .colorCount = 0
};

GK_EXPORT
GkOutput*
gkDefaultRenderOut(void) {
  return &gk_def_renderout;
}
