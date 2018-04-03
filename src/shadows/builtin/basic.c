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
#include "../../state/gpu.h"

#include <string.h>

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
  pass->output      = gkAllocOutput(gkContextOf(scene));
  pass->noMaterials = true;
  pass->noLights    = true;
  sm->pass          = pass;

  sm->viewProj     = malloc(sizeof(mat4));
  sm->splitc       = 1;

  gkBindOutput(scene, pass->output);

  if (light->type != GK_LIGHT_TYPE_POINT) {
    /* TODO: optimize it */
    sm->size.w = scene->viewport[2] * scene->backingScale;
    sm->size.h = scene->viewport[3] * scene->backingScale;

    gkAddDepthTexTarget(scene, pass, sm->size);
  } else {
    /* TODO: optimize it */
    sm->size.w = scene->viewport[2] * scene->backingScale;
    sm->size.h = sm->size.w;

    gkAddDepthCubeTexTarget(scene, pass, sm->size.w);
  }

  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    /* todo: handle error */
    printf("error: framebuff couldn't be created");
  }

  gkBindOutput(scene, scene->finalOutput);

  return sm;
}

void
gkRenderBasicShadowMap(GkScene * __restrict scene,
                       GkLight * __restrict light) {
  GkContext   *ctx;
  GkProgram   *prog;
  GkSceneImpl *sceneImpl;
  GkShadowMap *sm;
  GkFrustum   *frustum, subFrustum;
  GkPrimInst **prims;
  size_t       i, primc;

  ctx       = gkContextOf(scene);
  sceneImpl = (GkSceneImpl *)scene;

  if (!(sm = hash_get(sceneImpl->shadows, &light->type))) {
    sm = gkSetupShadows(scene, light);
    hash_set(sceneImpl->shadows, &light->type, sm);
  }

  prog          = sm->pass->prog;
  sm->currLight = light;

  gkPushState(ctx);
  gkBindOutput(scene, sm->pass->output);

  gkEnableDepthTest(ctx);
  gkUseProgram(ctx, prog);

  frustum = &scene->camera->frustum;

  /* render point of view of light  */
  glCullFace(GL_FRONT); /* todo: add to gpu state */
  glViewport(0, 0, sm->size.w, sm->size.h);

  memcpy(&subFrustum, frustum, sizeof(subFrustum));
  subFrustum.transp = NULL;
  subFrustum.opaque = NULL;

  if (light->type != GK_LIGHT_TYPE_POINT) {
    gkShadowMatrix(scene, light, sm->viewProj[0]);
    glClear(GL_DEPTH_BUFFER_BIT);

    /* cull sub frustum */
    gkCullSubFrustum(frustum, &subFrustum);

    /* opaque */
    prims = subFrustum.opaque->items;
    primc = subFrustum.opaque->count;
    for (i = 0; i < primc; i++)
      gkRenderShadowMap(scene, sm, prims[i], prog, 0);

    /* transparent */
    prims = subFrustum.transp->items;
    primc = subFrustum.transp->count;
    for (i = 0; i < primc; i++)
      gkRenderShadowMap(scene, sm, prims[i], prog, 0);

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

    sm->near = 0.1f;   /* TODO: calculate it */
    sm->far  = 100.0f; /* TODO: calculate it */

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

      glm_frustum_planes(sm->viewProj[0], subFrustum.planes);

      /* cull sub frustum */
      gkCullSubFrustum(frustum, &subFrustum);

      /* opaque */
      prims = subFrustum.opaque->items;
      primc = subFrustum.opaque->count;
      for (i = 0; i < primc; i++)
        gkRenderShadowMap(scene, sm, prims[i], prog, 0);

      /* transparent */
      prims = subFrustum.transp->items;
      primc = subFrustum.transp->count;
      for (i = 0; i < primc; i++)
        gkRenderShadowMap(scene, sm, prims[i], prog, 0);
    }
  }

  if (subFrustum.opaque)
    free(subFrustum.opaque);

  if (subFrustum.transp)
    free(subFrustum.transp);

  glCullFace(GL_BACK);
  gkPopState(ctx);

  glViewport(0,
             0,
             scene->viewport[2] * scene->backingScale,
             scene->viewport[3] * scene->backingScale);
}
