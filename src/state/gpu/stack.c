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

GkGPUApplyStateFn gk__stateFuncs[] = {
  NULL,
  gkApplyDepthState,
  gkApplyBlendState,
  gkApplyTexState,
  gkApplyOutputState,
  gkApplyCullFaceState,
  gkApplyFrameBuffState
};

GK_EXPORT
void
gkPushState(GkContext * __restrict ctx) {
  GkStatesItem *newst;

  if (!ctx->states || !ctx->states->last)
    return;

  /* save space! */
  if (((GkStatesItem *)flist_last(ctx->states))->isempty)
    return;

  newst = calloc(1, sizeof(*newst));
  newst->isempty = true;

  flist_append(ctx->states, newst);
}

GK_EXPORT
void
gkPopState(GkContext * __restrict ctx) {
  GkStatesItem *curr;
  FListItem    *curri;
  GkStateBase  *prevst, *currst;

  curr = flist_pop(ctx->states);
  if (!curr)
    return;

  if (!(curri = curr->states))
    goto fr;

  /* revert each state to previous */
  do {
    currst = curri->data;
    prevst = currst->prev;

#ifdef DEBUG
    assert(prevst != currst);
    assert(prevst);
#endif

    gk__stateFuncs[prevst->type](ctx, prevst);

    free(currst);
  } while ((curri = curri->next));

fr:
  /* we are no longer need to current state */
  free(curr);
}
