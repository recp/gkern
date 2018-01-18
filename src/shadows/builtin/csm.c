/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../common.h"
#include "csm.h"
#include "../render.h"
#include "../../shader/shader.h"
#include "../../shader/builtin_shader.h"
#include "../../gpu_state/common.h"

#include <string.h>

extern mat4 gk__biasMatrix;

GkShadowMap*
gkSetupShadowMapCSM(GkScene * __restrict scene) {
  GkShadowMap *sm;
  GkPass      *pass;
  GLenum       status;
  int          splitc;

  sm   = calloc(1, sizeof(*sm));
  pass = calloc(1, sizeof(*pass));

  pass->prog        = gkBuiltinProg(GK_BUILTIN_PROG_SHADOWMAP);
  pass->output      = gkAllocPassOut();
  pass->noMaterials = true;
  pass->noLights    = true;
  sm->pass          = pass;

  splitc            = gkShadowSplit();
  sm->splitc        = splitc;
  sm->distances     = malloc(sizeof(float) * splitc);
  sm->viewProj      = malloc(sizeof(mat4)  * splitc);

  gkBindPassOut(scene, pass->output);
  gkPassEnableDepthTexArray(scene, pass, splitc);

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
gkRenderShadowMapCSM(GkScene * __restrict scene,
                     GkLight * __restrict light) {
  mat4             view, proj;
  vec4            *corners;
  GkContext       *ctx;
  GkProgram       *prog;
  GkSceneImpl     *sceneImpl;
  GkShadowMap     *sm;
  GkFrustum       *frustum, subFrustum;
  vec3             box[2], boxInFrustum[2], finalBox[2];
  size_t           j;
  uint32_t         i, m;
  GLint            depth;
  float            n, f, im, Clog, Cuni, C, p22, p32;

  ctx       = gkContextOf(scene);
  sceneImpl = (GkSceneImpl *)scene;

  if (!(sm = sceneImpl->shadows))
    sceneImpl->shadows = sm = gkSetupShadows(scene);

  prog          = sm->pass->prog;
  sm->currLight = light;

  gkPushState(ctx);
  gkBindPassOut(scene, sm->pass->output);

  gkUseProgram(ctx, prog);
  gkEnableDepthTest(ctx);
  glCullFace(GL_FRONT);

  glm_persp_decomp_z(scene->camera->proj, &n, &f);

  frustum  = &scene->camera->frustum;
  corners  = frustum->corners;
  p22      = scene->camera->proj[2][2];
  p32      = scene->camera->proj[3][2];

  m        = sm->splitc;
  depth    = sm->pass->output->depth;

  glm_look_anyup(scene->camera->frustum.center, light->dir, view);

  memcpy(&subFrustum, frustum, sizeof(subFrustum));
  for (i = 0; i < m; i++) {
    /* prepare subFrustum for next */
    subFrustum.objs      = NULL;
    subFrustum.objsCount = 0;
    subFrustum.objsLen   = 0;

    /* split sceheme: PSSM */
    im   = (float)(i + 1) / m;
    Clog = n * powf(f / n, im);
    Cuni = n + (f - n) * im;
    C    = (Clog + Cuni) * 0.5;

    sm->distances[i] = 0.5f * (-C * p22 + p32) / C + 0.5f;

    /* update planes */
    subFrustum.planes[5][3] = C;

    /* cull sub frustum */
    gkCullSubFrustum(frustum, &subFrustum);
    if (subFrustum.objsCount == 0)
      continue;

    /* prepare projection maxtix for subFrustum */
    glm_frustum_corners_at(corners, C, f, &subFrustum.corners[4]);
    glm_frustum_box(subFrustum.corners, view, box);

    gkBoxInFrustum(&subFrustum, boxInFrustum);
    glm_aabb_transform(boxInFrustum, view, boxInFrustum);
    glm_aabb_crop(box, boxInFrustum, finalBox);

    /* TODO: why 50? is it enough for large scenes ?*/
    glm_ortho_aabb_pz(finalBox, f, proj);

    glm_mat4_mul(proj, view, sm->viewProj[i]);

    /* render scene light's POV */
    glFramebufferTextureLayer(GL_FRAMEBUFFER,
                              GL_DEPTH_ATTACHMENT,
                              depth,
                              0,
                              i);

    glClear(GL_DEPTH_BUFFER_BIT);
    for (j = 0; j < subFrustum.objsCount; j++)
      gkRenderShadowMap(scene, subFrustum.objs[j], prog, i);

    /* push near to next's far */
    subFrustum.planes[4][3] = subFrustum.planes[5][3];
    memcpy(subFrustum.corners[0],
           subFrustum.corners[4],
           sizeof(subFrustum.corners[0]) * 4);

    /* prepare shadow matrix for actual rendering pass */
    glm_mat4_mul(gk__biasMatrix,
                 sm->viewProj[i],
                 sm->viewProj[i]);
  }

  glCullFace(GL_BACK);
  gkPopState(ctx);
}
