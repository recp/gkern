/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "common.h"

GK_EXPORT
void
gkEnableDepthTest(GkContext * __restrict ctx) {
  GkDepthState *state;

  state = gkGetOrCreatState(ctx, GK_GPUSTATE_DEPTH);
  if (state->test)
    return;

  state->test = true;

  glEnable(GL_DEPTH_TEST);
}

GK_EXPORT
void
gkDisableDepthTest(GkContext * __restrict ctx) {
  GkDepthState *state;

  state = gkGetOrCreatState(ctx, GK_GPUSTATE_DEPTH);
  if (!state->test)
    return;

  state->test = false;

  glDisable(GL_DEPTH_TEST);
}

GK_EXPORT
void
gkDepthMask(GkContext * __restrict ctx, GLboolean flag) {
  GkDepthState *state;

  state = gkGetOrCreatState(ctx, GK_GPUSTATE_DEPTH);
  if (state->mask == flag)
    return;

  state->mask = flag;

  glDepthMask(flag);
}

GK_EXPORT
void
gkDepthFunc(GkContext * __restrict ctx, GLenum func) {
  GkDepthState *state;

  state = gkGetOrCreatState(ctx, GK_GPUSTATE_DEPTH);
  if (state->func == func)
    return;

  state->func = func;

  glDepthFunc(func);
}
