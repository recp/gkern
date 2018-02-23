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
gkDisableCullFace(GkContext * __restrict ctx) {
  GkFaceState *state;

  state = gkGetOrCreatState(ctx, GK_GPUSTATE_CULLFACE);
  if (!state->cull)
    return;

  state->cull = false;
  glDisable(GL_CULL_FACE);
}

GK_EXPORT
void
gkEnableCullFace(GkContext * __restrict ctx) {
  GkFaceState *state;

  state = gkGetOrCreatState(ctx, GK_GPUSTATE_CULLFACE);
  if (state->cull)
    return;

  state->cull = true;
  glEnable(GL_CULL_FACE);
}

GK_EXPORT
void
gkCullFace(GkContext * __restrict ctx, GLenum face) {
  GkFaceState *state;

  state = gkGetOrCreatState(ctx, GK_GPUSTATE_CULLFACE);

  state->face = face;
  glCullFace(face);
}

GK_EXPORT
void
gkFrontFace(GkContext * __restrict ctx, GLenum face) {
  GkFaceState *state;

  state = gkGetOrCreatState(ctx, GK_GPUSTATE_CULLFACE);

  state->frontFace = face;
  glFrontFace(face);
}
