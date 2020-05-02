/*
 * This file is part of the gk project (https://github.com/recp/gk)
 * Copyright (c) Recep Aslantas.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
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
