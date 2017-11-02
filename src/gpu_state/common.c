/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "common.h"

#include <ds/forward-list.h>
#include <string.h>
#include <stdlib.h>

_gk_hide
void*
gkGetOrCreatState(GkContext * __restrict ctx,
                  GkGPUStateType         type) {
  FListItem *item;
  
  item = flist_last(ctx->states);
  while (item) {
    GkStateBase *state;
    
    state = item->data;
    if (state->type == type)
      return state;
    item = item->next;
  }
  
  if (type != GK_GPUSTATE_TEXTURE)
    return gkCreatStateFromCurrent(ctx, type);
  
  return NULL;
}

_gk_hide
void*
gkGetOrCreatTexState(GkContext * __restrict ctx,
                     uint32_t               arrayIndex,
                     GLenum                 target) {
  FListItem *item;
  
  item = flist_last(ctx->states);
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
  
  return gkCreatTexStateFromCurrent(ctx, arrayIndex, target);
}

_gk_hide
GkStateBase*
gkCreatStateFromCurrent(GkContext * __restrict ctx,
                        GkGPUStateType         type) {
  GkStateBase *state;
  GkGPUStates *curr;
  void        *ptr;
  size_t       len;
  
  curr  = ctx->currState;
  state = calloc(sizeof(*state), 1);
  
  switch (type) {
    case GK_GPUSTATE_DEPTH:
      ptr = &curr->depthState;
      len = sizeof(GkDepthState);
      break;
    case GK_GPUSTATE_BLEND:
      ptr = &curr->blendState;
      len = sizeof(GkBlendState);
      break;
    default:
      free(state);
      return NULL;
  }
  
  memcpy(state, ptr, len);
  flist_insert(ctx->states, state);
  
  return state;
}

_gk_hide
GkStateBase*
gkCreatTexStateFromCurrent(GkContext * __restrict ctx,
                           uint32_t               index,
                           GLenum                 target) {
  GkTextureState *state, *statei;
  GkGPUStates    *curr;
  
  curr   = ctx->currState;
  state  = calloc(sizeof(*state), 1);
  
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
    statei->base.next = &curr->texStates->base;
  curr->texStates = statei;
  
  flist_insert(ctx->states, state);
  
  return &state->base;
}
