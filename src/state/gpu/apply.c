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

  if (depthState->test != cst->depth->test) {
    if (depthState->test)
      glEnable(GL_DEPTH_TEST);
    else
      glDisable(GL_DEPTH_TEST);
  }
  
  if (depthState->func != cst->depth->func)
    glDepthFunc(depthState->func);

  if (depthState->mask != cst->depth->mask)
    glDepthMask(depthState->mask);

  ctx->currState->depth = depthState;
}

_gk_hide
void
gkApplyBlendState(GkContext   * __restrict ctx,
                  GkStateBase * __restrict st) {
  GkBlendState *blendState;
  GkGPUStates  *cst;
  
  cst        = ctx->currState;
  blendState = (GkBlendState *)st;

  if (blendState->blend != cst->blend->blend) {
    if (blendState->blend)
      glEnable(GL_BLEND);
    else
      glDisable(GL_BLEND);
  }

  if (blendState->src != cst->blend->src
      || blendState->dst != cst->blend->dst) {
    if (!st->indexed) {
      glBlendFunc(blendState->src, blendState->dst);
    } else {
      glBlendFunci(blendState->base.index, blendState->src, blendState->dst);
    }
  }

  if (blendState->eq != cst->blend->eq) {
    glBlendEquation(blendState->eq);
  }

  ctx->currState->blend = blendState;
}

_gk_hide
void
gkApplyTexState(GkContext   * __restrict ctx,
                GkStateBase * __restrict st) {
  GkTextureState *texState, *currtex;
  GkGPUStates    *cst;
  HTable         *texu;
  bool            chTex;
  
  cst      = ctx->currState;
  texState = (GkTextureState *)st;
  chTex    = true;
  texu     = hash_get(cst->tex, DS_ITOP(texState->texunit));

  if (texState->texunit != cst->activeTex)
    gkActiveTexture(ctx, texState->texunit);

  if ((currtex = hash_get(texu, DS_ITOP(texState->target))))
    chTex = currtex->texid != texState->texid;

  hash_set(texu, DS_ITOP(texState->target), st);

  if (chTex)
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

  if (cst->output->renderOutput != outputState->renderOutput) {
    cst->output->renderOutput = outputState->renderOutput;
    glBindFramebuffer(GL_FRAMEBUFFER, outputState->renderOutput->fbo);
  }

  ctx->currState->output = outputState;
}

_gk_hide
void
gkApplyCullFaceState(GkContext   * __restrict ctx,
                     GkStateBase * __restrict st) {
  GkFaceState *faceState;
  GkGPUStates *cst;

  cst           = ctx->currState;
  faceState = (GkFaceState *)st;

  if (cst->face->face != faceState->cull) {
    if (faceState->cull)
      glEnable(GL_CULL_FACE);
    else
      glDisable(GL_CULL_FACE);

    cst->face->cull = faceState->cull;
  }

  if (cst->face->face == faceState->face)  {
    glCullFace(faceState->face);
    cst->face->face = faceState->face;
  }

  if (cst->face->frontFace == faceState->frontFace)  {
    glFrontFace(faceState->frontFace);
    cst->face->frontFace = faceState->frontFace;
  }

  ctx->currState->face = faceState;
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
