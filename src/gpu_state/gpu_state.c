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
  bool   depthTest:1;
  bool   blend:1;
  GLenum depthFunc;
  GLenum blendEq;
  GLenum blendFuncSrc;
  GLenum blendFuncDest;
} GkGPUState;

static GkGPUState gk__defgpustate = {
  true,
  false,
  GL_LESS,
  GL_FUNC_ADD,
  GL_ONE,
  GL_ZERO
};

static
GkGPUState*
gk__gpustate(GkContext * __restrict ctx);

static
GkGPUState*
gk__gpustate(GkContext * __restrict ctx) {
  GkGPUState *state;
  
  if (!(state = flist_last(ctx->states))) {
    ctx->currState = state = calloc(sizeof(*state), 1);
    memcpy(state, &gk__defgpustate, sizeof(gk__defgpustate));
  }

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
  
  if (state->depthFunc != curr->depthTest)
    glDepthFunc(state->depthFunc);
  
  if (state->blend != curr->blend) {
    if (curr->blend) {
      glDisable(GL_BLEND);
    } else {
      glEnable(GL_BLEND);
    }
  }
  
  if (state->blendFuncSrc != curr->blendFuncSrc
      || state->blendFuncDest != curr->blendFuncDest) {
    glBlendFunc(state->blendFuncSrc, state->blendFuncDest);
  }
  
  if (state->blendEq != curr->blendEq)
    glBlendEquation(state->blendEq);

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
