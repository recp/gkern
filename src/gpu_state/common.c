/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "common.h"

#include <ds/forward-list.h>
#include <ds/forward-list-sep.h>
#include <string.h>
#include <stdlib.h>

_gk_hide
void*
gkGetOrCreatState(GkContext * __restrict ctx,
                  GkGPUStateType         type) {
  GkStatesItem *sti;
  FListItem    *item;

  sti = flist_last(ctx->states);
  if (!sti) {
    sti = calloc(1, sizeof(*sti));
    flist_insert(ctx->states, sti);
  }

  item = sti->states;
  while (item) {
    GkStateBase *state;

    state = item->data;
    if (state->type == type)
      return state;
    item = item->next;
  }

  if (type != GK_GPUSTATE_TEXTURE)
    return gkCreatState(ctx, sti, type);

  return NULL;
}

_gk_hide
void*
gkGetOrCreatTexState(GkContext * __restrict ctx,
                     uint32_t               arrayIndex,
                     GLenum                 target) {
  GkStatesItem *sti;
  FListItem    *item;

  sti = flist_last(ctx->states);
  if (!sti) {
    sti = calloc(1, sizeof(*sti));
    flist_insert(ctx->states, sti);
  }

  item = sti->states;
  while (item) {
    GkStateBase *state;

    state = item->data;
    if (state->type == GK_GPUSTATE_TEXTURE
        && state->arrayIndex == arrayIndex) {
      GkTextureState *texState;
      texState = (GkTextureState *)state;
      if (texState->target == target)
        return state;
    }
    item = item->next;
  }

  return gkCreatTexState(ctx, sti, arrayIndex, target);
}

_gk_hide
GkStateBase*
gkCreatState(GkContext    * __restrict ctx,
             GkStatesItem * __restrict sti,
             GkGPUStateType            type) {
  GkStateBase *state;
  GkGPUStates *curr;
  void        *ptr;
  size_t       len;

  curr = ctx->currState;
  switch (type) {
    case GK_GPUSTATE_DEPTH:
      ptr = &curr->depthState;
      len = sizeof(GkDepthState);
      break;
    case GK_GPUSTATE_BLEND:
      ptr = &curr->blendState;
      len = sizeof(GkBlendState);
      break;
    case GK_GPUSTATE_RENDER_OUT:
      ptr = &curr->outputState;
      len = sizeof(GkRenderOutState);
      break;
    default:
      return NULL;
  }

  state = calloc(1, len);
  memcpy(state, ptr, len);

  state->type = type;

  sti->isempty = false;
  flist_sp_insert(&sti->states, state);

  return state;
}

_gk_hide
void
gkStateMakeCurrent(GkContext   * __restrict ctx,
                   GkStateBase * __restrict st) {
  GkGPUStates *ast;

  ast = ctx->currState;
  switch (st->type) {
    case GK_GPUSTATE_DEPTH:
      memcpy(&ast->depthState, st, sizeof(GkDepthState));
      break;
    case GK_GPUSTATE_BLEND:
      memcpy(&ast->blendState, st, sizeof(GkBlendState));
      break;
    case GK_GPUSTATE_RENDER_OUT:
      memcpy(&ast->outputState, st, sizeof(GkRenderOutState));
      break;
    default:
      return;
  }
}

_gk_hide
GkStateBase*
gkCreatTexState(GkContext    * __restrict ctx,
                GkStatesItem * __restrict sti,
                uint32_t                  index,
                GLenum                    target) {
  GkTextureState *state, *statei;
  GkGPUStates    *curr;

  curr   = ctx->currState;
  state  = calloc(1, sizeof(*state));

  state->base.arrayIndex = index;
  state->base.type       = GK_GPUSTATE_TEXTURE;
  state->target          = target;
  state->texunit         = index;

  statei = curr->texStates;
  while (statei) {
    if (statei->base.arrayIndex == index
        && statei->target == target) {
      state->texid = statei->texid;
      return &state->base;
    }
    statei = (GkTextureState *)statei->base.next;
  }

  if (curr->texStates)
    state->base.next = &curr->texStates->base;
  curr->texStates = statei;

  sti->isempty = false;
  flist_sp_insert(&sti->states, state);

  return &state->base;
}
