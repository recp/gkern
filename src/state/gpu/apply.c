/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../common.h"
#include "apply.h"
#include <ds/forward-list.h>

_gk_hide
void
gkApplyDepthState(GkContext   * __restrict ctx,
                  GkStateBase * __restrict st) {
  GkDepthState *depthState;
  GkGPUStates  *cst;
  
  cst        = ctx->currState;
  depthState = (GkDepthState *)st;

  if (depthState->test != cst->depthState->test) {
    if (depthState->test)
      glEnable(GL_DEPTH_TEST);
    else
      glDisable(GL_DEPTH_TEST);
  }
  
  if (depthState->func != cst->depthState->func)
    glDepthFunc(depthState->func);

  if (depthState->mask != cst->depthState->mask)
    glDepthMask(depthState->mask);

  ctx->currState->depthState = depthState;
}

_gk_hide
void
gkApplyBlendState(GkContext   * __restrict ctx,
                  GkStateBase * __restrict st) {
  GkBlendState *blendState;
  GkGPUStates  *cst;
  
  cst        = ctx->currState;
  blendState = (GkBlendState *)st;

  if (blendState->blend != cst->blendState->blend) {
    if (blendState->blend)
      glEnable(GL_BLEND);
    else
      glDisable(GL_BLEND);
  }

  if (blendState->src != cst->blendState->src
      || blendState->dst != cst->blendState->dst) {
    if (!st->indexed) {
      glBlendFunc(blendState->src, blendState->dst);
    } else {
      glBlendFunci(blendState->base.index, blendState->src, blendState->dst);
    }
  }

  if (blendState->eq != cst->blendState->eq) {
    glBlendEquation(blendState->eq);
  }

  ctx->currState->blendState = blendState;
}

_gk_hide
void
gkApplyTexState(GkContext   * __restrict ctx,
                GkStateBase * __restrict st) {
  GkStatesItem   *sti;
  GkTextureState *texState;
  GkGPUStates    *cst;
  FListItem      *item;
  
  cst      = ctx->currState;
  texState = (GkTextureState *)st;
  
  if (texState->base.index != cst->activeTex)
    glActiveTexture(GL_TEXTURE0 + texState->base.index);
  
  if (!(sti = flist_last(ctx->states)))
    return;

  /* todo: optimize this. */
  item = sti->states;
  while (item) {
    GkStateBase *state;
    
    state = item->data;
    if (state->type == GK_GPUSTATE_TEXTURE
        && state->index == texState->base.index) {
      GkTextureState *ts;
      ts = (GkTextureState *)state;
      
      /* no need to change texture */
      if (ts->target == texState->target
          && ts->texid == texState->texid)
        return;
    }
    item = item->next;
  }
  
  glBindTexture(texState->target, texState->texid);
}

_gk_hide
void
gkApplyOutputState(GkContext   * __restrict ctx,
                   GkStateBase * __restrict st) {
  GkOutputState *outputState;
  GkGPUStates      *cst;
  
  cst         = ctx->currState;
  outputState = (GkOutputState *)st;

  if (cst->outputState->renderOutput != outputState->renderOutput) {
    cst->outputState->renderOutput = outputState->renderOutput;
    glBindFramebuffer(GL_FRAMEBUFFER, outputState->renderOutput->fbo);
  }

  ctx->currState->outputState = outputState;
}

_gk_hide
void
gkApplyCullFaceState(GkContext   * __restrict ctx,
                     GkStateBase * __restrict st) {
  GkFaceState *faceState;
  GkGPUStates *cst;

  cst           = ctx->currState;
  faceState = (GkFaceState *)st;

  if (cst->faceState->face != faceState->cull) {
    if (faceState->cull)
      glEnable(GL_CULL_FACE);
    else
      glDisable(GL_CULL_FACE);

    cst->faceState->cull = faceState->cull;
  }

  if (cst->faceState->face == faceState->face)  {
    glCullFace(faceState->face);
    cst->faceState->face = faceState->face;
  }

  if (cst->faceState->frontFace == faceState->frontFace)  {
    glFrontFace(faceState->frontFace);
    cst->faceState->frontFace = faceState->frontFace;
  }

  ctx->currState->faceState = faceState;
}

_gk_hide
void
gkApplyFrameBuffState(GkContext   * __restrict ctx,
                      GkStateBase * __restrict st) {
  GkFramebuffState *frmState;
  GkGPUStates      *cst;

  cst      = ctx->currState;
  frmState = (GkFramebuffState *)st;

  if (cst->frame->drawbuff != frmState->drawbuff)
    glDrawBuffer(frmState->drawbuff);

  if (cst->frame->readbuff != frmState->readbuff)
    glDrawBuffer(frmState->readbuff);

  ctx->currState->frame = frmState;
}
