/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
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
  GkProgram   *prog;
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

  if (ctx->currState->prog != prog)
    gkUseProgram(ctx, prog);

  if (prog->lastMaterial != material && !pass->noMaterials)
    gkUniformMaterial(ctx, prog, material);

  gkUniformTransform(prog, trans, scene->camera);

  if (!pass->noLights) {
    switch (sceneImpl->rpath) {
      case GK_RNPATH_MODEL_PERLIGHT:
        if (!material->transparent)
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
