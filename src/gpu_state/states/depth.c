/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../common.h"
#include "../../../include/gk/gk.h"
#include "../../../include/gk/gpu_state.h"

#include "common.h"
#include "apply.h"

#include <ds/forward-list.h>
#include <string.h>

GK_EXPORT
void
gkEnableDepthTest(GkContext * __restrict ctx) {
  GkDepthState *state;

  state = gkGetOrCreatState(ctx, GK_GPUSTATE_DEPTH);
  if (state->depthTest)
    return;

  state->depthTest = true;
  glEnable(GL_DEPTH_TEST);
}

GK_EXPORT
void
gkDisableDepthTest(GkContext * __restrict ctx) {
  GkDepthState *state;

  state = gkGetOrCreatState(ctx, GK_GPUSTATE_DEPTH);
  if (!state->depthTest)
    return;

  state->depthTest = false;
  glDisable(GL_DEPTH_TEST);
}
