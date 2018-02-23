/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "../../include/gk/prims/builtin-prim.h"

#include "shadows.h"
#include "../shader/builtin_shader.h"
#include "../program/uniform_cache.h"
#include "../state/gpu.h"

void
gkRenderShadowMapTo(GkScene   * __restrict scene,
                    GkPassOut * __restrict output) {
  GkContext   *ctx;
  GkSceneImpl *sceneImpl;
  GkShadowMap *sm;
  GkCamera    *cam;
  GkProgram   *depthProg;
  int32_t      texUnit;
  float        near, far;

  sceneImpl = (GkSceneImpl *)scene;
  sm = sceneImpl->shadows;
  if (!sm->currLight || !(cam = sm->currLight->camera))
    return;

  ctx = gkContextOf(scene);

  gkPushState(ctx);
  gkBindPassOut(scene, output);

  texUnit   = (int32_t)ctx->samplers->count;
  depthProg = gkBuiltinProg(GK_BUILTIN_PROG_DEPTH);

  glm_persp_decomp_z(cam->proj, &near, &far);

  gkUseProgram(gkContextOf(scene), depthProg);
  glActiveTexture(GL_TEXTURE0 + texUnit);
  glBindTexture(GL_TEXTURE_2D, sm->pass->output->depth);
  gkUniform1i(depthProg, "uColorTex", texUnit);
  gkUniform1f(depthProg, "uNear",     near);
  gkUniform1f(depthProg, "uFar",      far);

  gkRenderBuiltinPrim(scene, GK_PRIM_TEXQUAD);
  gkPopState(ctx);
}
