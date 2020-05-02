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

#include "../common.h"
#include "../../include/gk/prims/builtin-prim.h"

#include "shadows.h"
#include "../shader/builtin_shader.h"
#include "../program/uniform_cache.h"
#include "../state/gpu.h"

void
gkRenderShadowMapTo(GkScene  * __restrict scene,
                    GkOutput * __restrict output) {
  GkContext   *ctx;
  GkSceneImpl *sceneImpl;
  GkShadowMap *sm;
  GkCamera    *cam;
  GkPipeline   *depthProg;
  int32_t      texUnit;
  float        near, far;

  sceneImpl = (GkSceneImpl *)scene;
  sm = sceneImpl->shadows;
  if (!sm->currLight || !(cam = sm->currLight->camera))
    return;

  ctx = gkContextOf(scene);

  gkPushState(ctx);
  gkBindOutput(scene, output);

  texUnit   = (int32_t)ctx->samplers->count;
  depthProg = gkBuiltinProg(GK_BUILTIN_PROG_DEPTH);

  glm_persp_decomp_z(cam->proj, &near, &far);

  gkUseProgram(gkContextOf(scene), depthProg);
  gkBindTextureTo(ctx, texUnit, GL_TEXTURE_2D, sm->pass->output->depth);
  gkUniform1i(depthProg, "uColorTex", texUnit);
  gkUniform1f(depthProg, "uNear",     near);
  gkUniform1f(depthProg, "uFar",      far);

  gkRenderBuiltinPrim(scene, GK_PRIM_TEXQUAD);
  gkPopState(ctx);
}
