/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../common.h"
#include "basic.h"
#include "../../shader/shader.h"
#include "../../shader/builtin_shader.h"
#include "../../gpu_state/common.h"

GkShadowMap*
gkSetupBasicShadowMap(GkScene * __restrict scene) {
  GkShadowMap *shadowMap;
  GkPass      *shadowPass;
  GLenum       status;

  shadowMap  = calloc(sizeof(*shadowMap), 1);
  shadowPass = calloc(sizeof(*shadowMap->shadowPass), 1);

  shadowPass->prog        = gkBuiltinProg(GK_BUILTIN_PROG_SHADOWMAP);
  shadowPass->output      = gkAllocPassOut();
  shadowPass->noMaterials = true;
  shadowPass->noLights    = true;
  shadowMap->shadowPass   = shadowPass;

  gkBindPassOut(scene, shadowPass->output);
  gkPassEnableDepthTex(scene, shadowPass);

  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    /* todo: handle error */
    printf("error: framebuff couldn't be created");
  }

  gkBindPassOut(scene, scene->finalOutput);

  return shadowMap;
}

void
gkRenderBasicShadowMap(GkScene * __restrict scene,
                       GkLight * __restrict light) {
  GkContext            *ctx;
  GkScenePrivateFields *scenePriv;
  GkShadowMap          *shadowMap;
  GkPass               *overridePass;
  GkCamera             *lightCam, *prevCam;
  GkRenderModelFn       renderModelFn;

  ctx = gkContextOf(scene);
  scenePriv = &scene->_priv;
  if (!(shadowMap = scenePriv->shadows))
    scene->_priv.shadows = shadowMap = gkSetupShadows(scene);

  shadowMap->currLight = light;
  renderModelFn = scene->renderModelFn;
  overridePass  = scenePriv->overridePass;
  prevCam       = scene->camera;

  gkPushState(scenePriv->ctx);
  gkBindPassOut(scene, shadowMap->shadowPass->output);

  gkEnableDepthTest(ctx);

  glClearDepth(1.0f);
  glClear(GL_DEPTH_BUFFER_BIT);

  /* todo: add these to gpu state */
  scene->renderModelFn      = gkRnModelNoMatOPass;
  scene->_priv.overridePass = shadowMap->shadowPass;
  lightCam                  = gkCameraOfLight(scene, light);
  scene->flags             &= ~GK_SCENEF_SHADOWS;

  /* render point of view of light  */
  glCullFace(GL_FRONT); /* todo: add to gpu state */
  gkSetCamera(scene, lightCam);
  gkRenderNode(scene, scene->rootNode, scene->trans);
  glCullFace(GL_BACK);

  /* restore states: todo add these to state manager */
  scene->renderModelFn      = renderModelFn;
  scene->_priv.overridePass = overridePass;
  scene->flags             |= GK_SCENEF_SHADOWS;

  gkSetCamera(scene, prevCam);
  gkBindPassOut(scene, scene->finalOutput);
  gkPopState(scenePriv->ctx);
}
