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
#include "../../../include/gk/prims/builtin-prim.h"
#include "../../../include/gk/gpu_state.h"
#include "../../shader/builtin_shader.h"
#include "../../program/uniform_cache.h"
#include "../../state/gpu.h"

#include "rn_light.h"
#include "rn_material.h"
#include "rn_pass.h"
#include "rn_prim.h"

void
gkRenderPass(GkScene    * __restrict scene,
             GkPrimInst * __restrict primInst,
             GkPass     * __restrict pass) {
  GkContext   *ctx;
  GkPipeline   *prog;
  GkSceneImpl *sceneImpl;
  GkTransform *trans;
  GkPrimitive *prim;
  GkMaterial  *material;

  if (!(prog = pass->prog))
    return;

  sceneImpl = (GkSceneImpl *)scene;
  ctx       = gkContextOf(scene);
  prim      = primInst->prim;
  trans     = primInst->trans;
  material  = primInst->activeMaterial;

  gkUseProgram(ctx, prog);

  if (!pass->noMaterials) {
    gkUniformMaterial(ctx, prog, material);
    gkToggleDoubleSided(ctx, material->doubleSided);
  }

  gkUniformTransform(prog, trans, scene->camera);

  if (!pass->noLights) {
    switch (sceneImpl->rpath) {
      case GK_RNPATH_MODEL_PERLIGHT:
        if (material->technique
            && !material->technique->transparent)
          gkRenderPrimPerLight(scene, prim, prog);
        else
          gkRenderTranspPrimPerLight(scene, prim, prog);
        break;
      case GK_RNPATH_SCENE_PERLIGHT:
        gkRenderPrimForLight(scene, trans, prim, prog);
        break;
      default: break;
    }
  } else {
    gkRenderPrim(scene, prim);
  }
}
