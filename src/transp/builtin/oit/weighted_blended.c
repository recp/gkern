/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../../../include/gk/pass.h"
#include "../../../common.h"
#include "../../../render/realtime/rn_prim.h"
#include "../../../shader/builtin_shader.h"
#include "weighted_blended.h"

GkClearOp GK__TRANS_WEIGBL_ACCUM_CLEAR = {
  .color      = &((vec4){0.0f, 0.0f, 0.0f, 0.0f}),
  .depth      = 1.0f,
  .clearColor = true,
  .clearDepth = false,
  .enabled    = true
};

GkClearOp GK__TRANS_WEIGBL_REVEALAGE_CLEAR = {
  .color      = &((vec4){1.1f, 1.1f, 1.1f, 1.1f}),
  .depth      = 1.0f,
  .clearColor = true,
  .clearDepth = false,
  .enabled    = true
};

GkBlendOp GK__TRANS_WEIGBL_ACCUM_BLEND = {
  .enabled    = true,
  .src        = GL_ONE,
  .dst        = GL_ONE
};

GkBlendOp GK__TRANS_WEIGBL_REVEALAGE_BLEND = {
  .enabled    = true,
  .src        = GL_ONE,
  .dst        = GL_ONE_MINUS_SRC_COLOR
};

void
gkTranspWeightedBlendedInit(GkScene * __restrict scene) {
  GkSceneImpl             *sceneImpl;
  GkTranspWeightedBlended *transp;
  GkPass                  *opaquePass, *transpPass;
  GkPassOutColor          *accum, *revealage;

  sceneImpl          = (GkSceneImpl *)scene;
  sceneImpl->transp  = transp = calloc(1, sizeof(*transp));
  transp->base.type  = GK_TRANSP_WEIGHTED_BLENDED;

  /* opaque pass */
  transp->opaquePass = opaquePass = gkAllocPass();
  gkBindPassOut(scene, opaquePass->output);
  gkPassEnableDepth(scene, opaquePass);
  gkAddRenderTarget(scene, opaquePass, GL_RGBA32F, GL_RGBA, GL_FLOAT);

  /* transparency pass */
  transp->transpPass = transpPass = gkAllocPass();
  gkBindPassOut(scene, transpPass->output);

  /* use opaque's depth */
  glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                            GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER,
                            opaquePass->output->depth);

  /* accum */
  gkAddRenderTarget(scene, transpPass, GL_RGBA16F, GL_RGBA, GL_HALF_FLOAT);
  accum            = transpPass->output->color;
  accum->clear     = &GK__TRANS_WEIGBL_ACCUM_CLEAR;
  accum->blend     = &GK__TRANS_WEIGBL_ACCUM_BLEND;

  /* revealage */
  gkAddRenderTarget(scene, transpPass, GL_R16F, GL_RED, GL_HALF_FLOAT);

  revealage        = accum->next;
  revealage->clear = &GK__TRANS_WEIGBL_REVEALAGE_CLEAR;
  revealage->blend = &GK__TRANS_WEIGBL_REVEALAGE_BLEND;

  /* compositing pass */
  transp->composProg = gkBuiltinProg(GK_BUILTIN_PROG_WEIGBL_COMPOS);
}

void
gkTranspWeightedBlended(GkScene * __restrict scene) {
  GkSceneImpl             *sceneImpl;
  GkTranspWeightedBlended *transp;
  GkPass                  *tpass;
  GkFrustum               *frustum;
  GkProgram               *composProg;
  float                    w, h;

  sceneImpl  = (GkSceneImpl *)scene;
  frustum    = &scene->camera->frustum;

  if (!(transp = sceneImpl->transp))
    gkTranspWeightedBlendedInit(scene);

  tpass      = transp->transpPass;
  composProg = transp->composProg;

  /* opaque pass */
  gkBindPassOut(scene, transp->opaquePass->output);
  glClearColor(1, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  gkRenderPrims(scene, frustum->opaque);

  /* transparent pass */
  sceneImpl->transpPass = false;
  gkBindPassOut(scene, tpass->output);

  /* TODO: add to state manager */
  glDepthMask(GL_FALSE);
  glDisable(GL_BLEND);
  glEnable(GL_CULL_FACE);

  glBlendFunci(0, GL_ONE,  GL_ONE);
  glBlendFunci(1, GL_ZERO, GL_ONE_MINUS_SRC_COLOR);

  gkClearColors(tpass->output);
  gkRenderPrims(scene, frustum->transp);

  sceneImpl->transpPass = false;

  /* compositing pass */
  gkBindPassOut(scene, scene->finalOutput);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, transp->opaquePass->output->fbo);
  glReadBuffer(GL_COLOR_ATTACHMENT0);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

  w = scene->vrect.size.w * scene->backingScale;
  h = scene->vrect.size.h * scene->backingScale;

  glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST);

  gkUseProgram(gkContextOf(scene), transp->composProg);
  gkBindRenderTargetTo(scene, tpass, 0, composProg, 0, "uAccum");
  gkBindRenderTargetTo(scene, tpass, 1, composProg, 1, "uReveal");

  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);

  glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
  gkRenderBuiltinPrim(scene, GK_PRIM_TEXQUAD);

  /*
   Alternative to blit

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glDisable(GL_BLEND);
   gkRenderTexture(scene, transp->opaquePass);
  */

  glDepthMask(GL_TRUE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glDepthFunc(GL_LESS);
  glDrawBuffer(GL_BACK);
  glEnable(GL_CULL_FACE);
}
