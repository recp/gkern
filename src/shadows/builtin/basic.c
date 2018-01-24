/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../common.h"
#include "basic.h"

#include "../render.h"
#include "../shad_common.h"
#include "../../shader/shader.h"
#include "../../shader/builtin_shader.h"
#include "../../gpu_state/common.h"

extern mat4 gk__biasMatrix;

GkShadowMap*
gkSetupBasicShadowMap(GkScene * __restrict scene,
                      GkLight * __restrict light) {
  GkShadowMap *sm;
  GkPass      *pass;
  GLenum       status;

  sm   = calloc(1, sizeof(*sm));
  pass = calloc(1, sizeof(*sm->pass));

  pass->prog        = gkBuiltinProg(GK_BUILTIN_PROG_SHADOWMAP);
  pass->output      = gkAllocPassOut();
  pass->noMaterials = true;
  pass->noLights    = true;
  sm->pass          = pass;

  sm->viewProj     = malloc(sizeof(mat4));
  sm->splitc       = 1;

  gkBindPassOut(scene, pass->output);

  if (light->type != GK_LIGHT_TYPE_POINT)
    gkPassEnableDepthTex(scene, pass);
  else
    gkPassEnableDepthCubeTex(scene, pass);

  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    /* todo: handle error */
    printf("error: framebuff couldn't be created");
  }

  gkBindPassOut(scene, scene->finalOutput);

  return sm;
}

void
gkRenderBasicShadowMap(GkScene * __restrict scene,
                       GkLight * __restrict light) {
  GkContext   *ctx;
  GkProgram   *prog;
  GkSceneImpl *sceneImpl;
  GkShadowMap *sm;
  GkModelInst **objs;
  size_t       i, c;

  ctx       = gkContextOf(scene);
  sceneImpl = (GkSceneImpl *)scene;

  if (!(sm = hash_get(sceneImpl->shadows, &light->type))) {
    sm = gkSetupShadows(scene, light);
    hash_set(sceneImpl->shadows, &light->type, sm);
  }


//  if (!(sm = sceneImpl->shadows))
//    sceneImpl->shadows = sm = gkSetupShadows(scene, light);

  prog          = sm->pass->prog;
  sm->currLight = light;

  gkPushState(ctx);
  gkBindPassOut(scene, sm->pass->output);

  gkEnableDepthTest(ctx);

  if (ctx->currState->prog != prog)
    gkUseProgram(ctx, prog);

  /* todo: no extra cull required for directional but cull for others! */
  objs = scene->camera->frustum.objs;
  c    = scene->camera->frustum.objsCount;

  /* render point of view of light  */
  glCullFace(GL_FRONT); /* todo: add to gpu state */

  if (light->type != GK_LIGHT_TYPE_POINT) {
    gkShadowMatrix(scene, light, sm->viewProj[0]);
    glClear(GL_DEPTH_BUFFER_BIT);

    for (i = 0; i < c; i++)
      gkRenderShadowMap(scene, sm, objs[i], prog, 0);

    glm_mat4_mul(gk__biasMatrix, sm->viewProj[0], sm->viewProj[0]);
  } else {
    mat4 proj, view;
    int  side;
    vec3 pos;

    struct cubeData{ vec3 dir; vec3 up; } cubeData[6] = {
      {{ 1.0f, 0.0f, 0.0f}, {0.0f,-1.0f, 0.0f}},
      {{-1.0f, 0.0f, 0.0f}, {0.0f,-1.0f, 0.0f}},
      {{ 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
      {{ 0.0f,-1.0f, 0.0f}, {0.0f, 0.0f,-1.0f}},
      {{ 0.0f, 0.0f, 1.0f}, {0.0f,-1.0f, 0.0f}},
      {{ 0.0f, 0.0f,-1.0f}, {0.0f,-1.0f, 0.0f}}
    };

    gkLightPos(scene, light, pos);

    glViewport(0,
               0,
               scene->vrect.size.w * scene->backingScale,
               scene->vrect.size.w * scene->backingScale);

    sm->near = 0.1f;  /* TODO: */
    sm->far = 100.0f; /* TODO: */

    glm_perspective(glm_rad(90.0f), 1.0f, sm->near, sm->far, proj);

    for (side = 0; side < 6; side++) {
      glFramebufferTexture2D(GL_FRAMEBUFFER,
                             GL_DEPTH_ATTACHMENT,
                             GL_TEXTURE_CUBE_MAP_POSITIVE_X + side,
                             sm->pass->output->depth,
                             0);

      glClear(GL_DEPTH_BUFFER_BIT);

      glm_look(pos, cubeData[side].dir, cubeData[side].up, view);
      glm_mat4_mul(proj, view, sm->viewProj[0]);

      for (i = 0; i < c; i++)
        gkRenderShadowMap(scene, sm, objs[i], prog, 0);
    }

#ifdef DEBUG
    int status;
    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
      /* todo: handle error */
      printf("error: couldn't create framebuff for samplerCUBE");
    }
#endif
  }

  glCullFace(GL_BACK);
  gkPopState(ctx);

  glViewport(0,
             0,
             scene->vrect.size.w * scene->backingScale,
             scene->vrect.size.h * scene->backingScale);
}
