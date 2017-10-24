/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../include/gk/gk.h"
#include "../../include/gk/gpu_state.h"

#include <ds/forward-list.h>
#include <string.h>

typedef struct GkGPUState {
  bool depthTest;
} GkGPUState;

static
GkGPUState*
gk__gpustate(GkContext * __restrict ctx);

static
GkGPUState*
gk__gpustate(GkContext * __restrict ctx) {
  GkGPUState *state;
  
  if (!(state = flist_last(ctx->states)))
    ctx->currState = state = calloc(sizeof(*state), 1);

  return state;
}

GK_EXPORT
void
gkPushState(GkContext * __restrict ctx) {
  GkGPUState *state;
  
  state = malloc(sizeof(*state));
  memcpy(state, ctx->currState, sizeof(*state));
  
  flist_insert(ctx->states, state);
}

GK_EXPORT
void
gkPopState(GkContext * __restrict ctx) {
  GkGPUState *state, *curr;
  
  if (ctx->states->count < 1)
    return;

  curr = ctx->currState;
  
  /* switch to previous state */
  state = ctx->currState = flist_pop(ctx->states);
  
  /* apply previous states if they are not as same as current */
  if (state->depthTest != curr->depthTest) {
    if (curr->depthTest) {
      glDisable(GL_DEPTH_TEST);
    } else {
      glEnable(GL_DEPTH_TEST);
    }
  }
  
  /* we are no longer need to current state */
  free(curr);
}

GK_EXPORT
void
gkEnableDepthTest(GkContext * __restrict ctx) {
  GkGPUState *state;

  state = gk__gpustate(ctx);
  if (state->depthTest)
    return;

  glEnable(GL_DEPTH_TEST);
}

GK_EXPORT
void
gkDisableDepthTest(GkContext * __restrict ctx) {
  GkGPUState *state;
  
  state = gk__gpustate(ctx);
  if (!state->depthTest)
    return;
  
  glDisable(GL_DEPTH_TEST);
}
