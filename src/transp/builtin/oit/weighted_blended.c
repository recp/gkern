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

#include "../../../../include/gk/pass.h"
#include "../../../../include/gk/prims/cube.h"
#include "../../../common.h"
#include "../../../render/realtime/prim.h"
#include "../../../render/realtime/texture.h"
#include "../../../shader/builtin_shader.h"
#include "../../../state/gpu.h"
#include "weighted_blended.h"

GkClearOp GK__TRANS_WEIGBL_ACCUM_CLEAR = {
  .color      = &((vec4){0.0f, 0.0f, 0.0f, 0.0f}),
  .depth      = 1.0f,
  .clearColor = true,
  .clearDepth = false,
  .enabled    = true
};

GkClearOp GK__TRANS_WEIGBL_REVEALAGE_CLEAR = {
  .color      = &((vec4){1.0f, 1.0f, 1.0f, 1.0f}),
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
  .src        = GL_ZERO,
  .dst        = GL_ONE_MINUS_SRC_COLOR
};

void
gkTranspWeightedBlendedInit(GkScene * __restrict scene) {
  GkSceneImpl             *sceneImpl;
  GkTranspWeightedBlended *transp;
  GkPass                  *opaquePass, *transpPass;
  GkColorOutput           *accum, *revealage;

  sceneImpl          = (GkSceneImpl *)scene;
  sceneImpl->transp  = transp = calloc(1, sizeof(*transp));
  transp->base.type  = GK_TRANSP_WEIGHTED_BLENDED;

  /* opaque pass */
  transp->opaquePass = opaquePass = gkAllocPass(gkContextOf(scene));
  gkBindOutput(scene, opaquePass->output);

  gkAddDepthTarget(scene, opaquePass);
  gkAddRenderTarget(scene, opaquePass, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);

  /* transparency pass */
  transp->transpPass = transpPass = gkAllocPass(gkContextOf(scene));
  gkBindOutput(scene, transpPass->output);

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
  GkContext               *ctx;
  GkSceneImpl             *sceneImpl;
  GkTranspWeightedBlended *transp;
  GkPass                  *tpass;
  GkFrustum               *frustum;
  GkPipeline               *composProg;
  GkLight                 *light, *firstLight;

  ctx        = gkContextOf(scene);
  sceneImpl  = (GkSceneImpl *)scene;
  frustum    = &scene->camera->frustum;

  if (!(transp = sceneImpl->transp)) {
    gkTranspWeightedBlendedInit(scene);
    transp = sceneImpl->transp;
  }

  tpass      = transp->transpPass;
  composProg = transp->composProg;

  gkPushState(ctx);

  /* opaque pass */
  gkEnableDepthTest(ctx);

  gkBindOutput(scene, transp->opaquePass->output);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  gkDepthFunc(ctx, GL_LESS);
  gkDisableBlend(ctx);

  if ((scene->flags & GK_SCENEF_DRAW_BBOX))
    gkDrawBBox(scene,
               scene->bbox,
               scene->rootNode->trans->world);

  light      = (GkLight *)sceneImpl->pub.lights;
  firstLight = light;

  do {
    sceneImpl->forLight = light;
    if (light != firstLight) {
      gkDepthFunc(ctx, GL_EQUAL);
      gkEnableBlend(ctx);
      gkBlendEq(ctx, GL_FUNC_ADD);
      gkBlendFunc(ctx, GL_ONE, GL_ONE);
    }

    gkRenderPrims(scene, frustum->opaque);
  } while ((light = (GkLight *)light->ref.next));

  /* transparent pass */
  sceneImpl->transpPass = true;
  gkBindOutput(scene, tpass->output);

  gkDepthFunc(ctx, GL_LESS);
  gkDepthMask(ctx, GL_FALSE);
  gkEnableBlend(ctx);
  gkEnableCullFace(ctx); /* TODO: add option for this */

  gkBlendFunci(ctx, 0, GL_ONE,  GL_ONE);
  gkBlendFunci(ctx, 1, GL_ZERO, GL_ONE_MINUS_SRC_COLOR);

  gkClearColors(tpass->output);

  light      = (GkLight *)sceneImpl->pub.lights;
  firstLight = light;

  do {
    sceneImpl->forLight = light;
    if (light != firstLight) {
      glColorMaski(1, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
      glDisablei(GL_BLEND, 1);
    }

    gkRenderPrims(scene, frustum->transp);
  } while ((light = (GkLight *)light->ref.next));

  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

  sceneImpl->transpPass = false;

  /* compositing pass */
  gkBindOutput(scene, transp->opaquePass->output);
  gkDisableDepthTest(ctx);
  gkBlendFunc(ctx, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

  gkUseProgram(ctx, transp->composProg);
  gkBindRenderTargetTo(scene, tpass, 0, composProg, 0, "uAccum");
  gkBindRenderTargetTo(scene, tpass, 1, composProg, 1, "uReveal");
  gkRenderBuiltinPrim(scene, GK_PRIM_TEXQUAD);

  gkPopState(ctx);

  gkRenderTexture(scene, transp->opaquePass);

  if ((scene->flags & GK_SCENEF_DRAW_BONES))
    gkDrawBones(scene);
}
