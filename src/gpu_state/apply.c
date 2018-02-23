/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
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
  if (depthState->depthTest != ast->depthState.depthTest) {
    if (depthState->depthTest)
      glEnable(GL_DEPTH_TEST);
    else
      glDisable(GL_DEPTH_TEST);
  }
  
  if (depthState->depthFunc != ast->depthState.depthFunc)
    glDepthFunc(depthState->depthFunc);
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
  
  if (blendState->sfactor != ast->blendState.sfactor
      || blendState->dfactor != ast->blendState.dfactor)
    glBlendFunc(blendState->sfactor, blendState->dfactor);
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
  
  if (texState->base.arrayIndex != ast->activeTex)
    glActiveTexture(GL_TEXTURE0 + texState->base.arrayIndex);
  
  if (!(sti = flist_last(ctx->states)))
    return;

  /* todo: optimize this. */
  item = sti->states;
  while (item) {
    GkStateBase *state;
    
    state = item->data;
    if (state->type == GK_GPUSTATE_TEXTURE
        && state->arrayIndex == texState->base.arrayIndex) {
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
  GkCullFaceState *cullfaceState;
  GkGPUStates     *ast;

  ast           = ctx->currState;
  cullfaceState = (GkCullFaceState *)st;

  if (ast->cullfaceState.face != cullfaceState->cull) {
    if (cullfaceState->cull)
      glEnable(GL_CULL_FACE);
    else
      glDisable(GL_CULL_FACE);

    ast->cullfaceState.cull = cullfaceState->cull;
  }

  if (ast->cullfaceState.face == cullfaceState->face)  {
    glCullFace(cullfaceState->face);
    ast->cullfaceState.face = cullfaceState->face;
  }
}
