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

#include "../../common.h"
#include "csm.h"
#include "../render.h"
#include "../shad_common.h"
#include "../../shader/shader.h"
#include "../../shader/builtin_shader.h"
#include "../../state/gpu.h"

#include <string.h>

extern mat4 gk__biasMatrix;

GkShadowMap*
gkSetupShadowMapCSM(GkScene * __restrict scene,
                    GkLight * __restrict light) {
  GkShadowMap *sm;
  GkPass      *pass;
  GLenum       status;
  int          splitc;

  sm   = calloc(1, sizeof(*sm));
  pass = calloc(1, sizeof(*pass));

  pass->prog        = gkBuiltinProg(GK_BUILTIN_PROG_SHADOWMAP);
  pass->output      = gkAllocOutput(gkContextOf(scene));
  pass->noMaterials = true;
  pass->noLights    = true;
  sm->pass          = pass;

  splitc            = gkShadowSplit();
  sm->splitc        = splitc;
  sm->distances     = malloc(sizeof(float) * splitc);
  sm->viewProj      = malloc(sizeof(mat4)  * splitc);

  gkBindOutput(scene, pass->output);
  gkAddDepthTexArrayTarget(scene, pass, splitc);

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
gkRenderShadowMapCSM(GkScene * __restrict scene,
                     GkLight * __restrict light) {
  mat4             view, proj;
  vec4            *corners;
  GkContext       *ctx;
  GkPipeline       *prog;
  GkSceneImpl     *sceneImpl;
  GkShadowMap     *sm;
  GkFrustum       *frustum, subFrustum;
  GkPrimInst      **prims;
  size_t           primc;
  uint32_t         i, j, m;
  GLint            depth;
  float            n, f, im, Clog, Cuni, C, p22, p32;

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

  gkShadowViewMatrix(scene, light, &scene->camera->frustum, view);

  memcpy(&subFrustum, frustum, sizeof(subFrustum));
  subFrustum.transp = NULL;
  subFrustum.opaque = NULL;

  for (i = 0; i < m; i++) {
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
    if (subFrustum.opaque->count == 0 && subFrustum.transp->count == 0)
      continue;

    /* prepare projection matrix for subFrustum */
    glm_frustum_corners_at(corners, C, f, &subFrustum.corners[4]);

    gkShadowProjMatrix(light, &subFrustum, view, f, proj);
    glm_mat4_mul(proj, view, sm->viewProj[i]);

    /* render scene light's POV */
    glFramebufferTextureLayer(GL_FRAMEBUFFER,
                              GL_DEPTH_ATTACHMENT,
                              depth,
                              0,
                              i);

    glClear(GL_DEPTH_BUFFER_BIT);

    /* opaque */
    prims = subFrustum.opaque->items;
    primc = subFrustum.opaque->count;
    for (j = 0; j < primc; j++)
      gkRenderShadowMap(scene, sm, prims[j], prog, i);

    /* transparent */
    prims = subFrustum.transp->items;
    primc = subFrustum.transp->count;
    for (j = 0; j < primc; j++)
      gkRenderShadowMap(scene, sm, prims[j], prog, i);

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

  if (subFrustum.opaque)
    free(subFrustum.opaque);

  if (subFrustum.transp)
    free(subFrustum.transp);

  glCullFace(GL_BACK);
  gkPopState(ctx);
}
