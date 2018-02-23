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
  GkGPUStates  *ast;
  
  ast        = ctx->currState;
  depthState = (GkDepthState *)st;
  if (depthState->test != ast->depthState.test) {
    if (depthState->test)
      glEnable(GL_DEPTH_TEST);
    else
      glDisable(GL_DEPTH_TEST);
  }
  
  if (depthState->func != ast->depthState.func)
    glDepthFunc(depthState->func);

  if (depthState->mask != ast->depthState.mask)
    glDepthMask(depthState->mask);
}

_gk_hide
void
gkApplyBlendState(GkContext   * __restrict ctx,
                  GkStateBase * __restrict st) {
  GkBlendState *blendState;
  GkGPUStates  *ast;
  
  ast        = ctx->currState;
  blendState = (GkBlendState *)st;
  
  if (blendState->blend != ast->blendState.blend) {
    if (blendState->blend)
      glEnable(GL_BLEND);
    else
      glDisable(GL_BLEND);
  }
  
  if (blendState->src != ast->blendState.src
      || blendState->dst != ast->blendState.dst)
    glBlendFunc(blendState->src, blendState->dst);
}

_gk_hide
void
gkApplyTexState(GkContext   * __restrict ctx,
                GkStateBase * __restrict st) {
  GkStatesItem   *sti;
  GkTextureState *texState;
  GkGPUStates    *ast;
  FListItem      *item;
  
  ast      = ctx->currState;
  texState = (GkTextureState *)st;
  
  if (texState->base.index != ast->activeTex)
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
  GkRenderOutState *outputState;
  GkGPUStates      *ast;
  
  ast         = ctx->currState;
  outputState = (GkRenderOutState *)st;

  if (ast->outputState.renderOutput == outputState->renderOutput)
    return;
  
  ast->outputState.renderOutput = outputState->renderOutput;
  glBindFramebuffer(GL_FRAMEBUFFER, outputState->renderOutput->fbo);
}

_gk_hide
void
gkApplyCullFaceState(GkContext   * __restrict ctx,
                     GkStateBase * __restrict st) {
  GkFaceState *faceState;
  GkGPUStates *ast;

  ast           = ctx->currState;
  faceState = (GkFaceState *)st;

  if (ast->faceState.face != faceState->cull) {
    if (faceState->cull)
      glEnable(GL_CULL_FACE);
    else
      glDisable(GL_CULL_FACE);

    ast->faceState.cull = faceState->cull;
  }

  if (ast->faceState.face == faceState->face)  {
    glCullFace(faceState->face);
    ast->faceState.face = faceState->face;
  }

  if (ast->faceState.frontFace == faceState->frontFace)  {
    glFrontFace(faceState->frontFace);
    ast->faceState.frontFace = faceState->frontFace;
  }
}
