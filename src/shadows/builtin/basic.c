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

extern mat4 gk__biasMatrix;

GkShadowMap*
gkSetupBasicShadowMap(GkScene * __restrict scene) {
  GkShadowMap *shadowMap;
  GkPass      *shadowPass;
  GLenum       status;

  shadowMap  = calloc(1, sizeof(*shadowMap));
  shadowPass = calloc(1, sizeof(*shadowMap->shadowPass));

  shadowPass->prog        = gkBuiltinProg(GK_BUILTIN_PROG_SHADOWMAP);
  shadowPass->output      = gkAllocPassOut();
  shadowPass->noMaterials = true;
  shadowPass->noLights    = true;
  shadowMap->shadowPass   = shadowPass;

  shadowMap->viewProj     = malloc(sizeof(mat4));
  shadowMap->splitc       = 1;

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
  GkContext       *ctx;
  GkProgram       *prog;
  GkSceneImpl     *sceneImpl;
  GkShadowMap     *sm;
  GkModelInst     **objs;
  size_t           i, c;

  ctx       = gkContextOf(scene);
  sceneImpl = (GkSceneImpl *)scene;

  if (!(sm = sceneImpl->shadows))
    sceneImpl->shadows = sm = gkSetupShadows(scene);

  prog          = sm->shadowPass->prog;
  sm->currLight = light;

  gkPushState(ctx);
  gkBindPassOut(scene, sm->shadowPass->output);

  gkEnableDepthTest(ctx);

  glClear(GL_DEPTH_BUFFER_BIT);

  if (ctx->currState->prog != prog)
    gkUseProgram(ctx, prog);

  /* todo: add these to gpu state */
  gkTransformsForLight(scene, light, sm->viewProj, 1);

  scene->flags &= ~GK_SCENEF_SHADOWS;

  /* todo: no extra cull required for directional but cull for others! */
  objs = scene->camera->frustum.objs;
  c    = scene->camera->frustum.objsCount;

  /* render point of view of light  */
  glCullFace(GL_FRONT); /* todo: add to gpu state */

  for (i = 0; i < c; i++)
    gkRenderShadowMap(scene, objs[i], prog, 0);

  glCullFace(GL_BACK);

  /* restore states: todo add these to state manager */
  scene->flags |= GK_SCENEF_SHADOWS;
  scene->subCamera = NULL;

  glm_mat4_mul(gk__biasMatrix, sm->viewProj[0], sm->viewProj[0]);

  gkBindPassOut(scene, scene->finalOutput);
  gkPopState(ctx);
}
