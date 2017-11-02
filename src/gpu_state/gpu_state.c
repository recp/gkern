/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../include/gk/gk.h"
#include "../../include/gk/gpu_state.h"

#include "common.h"
#include "apply.h"

#include <ds/forward-list.h>
#include <string.h>

GkGPUApplyStateFn gk__stateFuncs[] = {
  NULL,
  gkApplyDepthState,
  gkApplyBlendState,
  gkApplyTexState
};

GK_EXPORT
void
gkPushState(GkContext * __restrict ctx) {
  if (!ctx->currState)
    return;

  flist_insert(ctx->states, ctx->currState);
}

GK_EXPORT
void
gkPopState(GkContext * __restrict ctx) {
  FListItem   *old, *curr, *oldi, *curri;
  GkStateBase *oldst, *newst;
  
  curri = curr = flist_last(ctx->states);
  old  = flist_pop(ctx->states);
  
  if (curri) {
    do {
      newst = curri->data;
      
      /* linear search, todo: */
      if ((oldi = old)) {
        do {
          oldst = oldi->data;
          
          if (oldst->type == newst->type
              && oldst->arrayIndex == newst->arrayIndex)
            goto foundst;
          oldi = oldi->next;
        } while (oldi);
      }

      continue;
    foundst:
      gk__stateFuncs[oldst->type](ctx, oldst);
    } while ((curri = curri->next));
  }

  /* we are no longer need to current state */
  free(curr);
}

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
