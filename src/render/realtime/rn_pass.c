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
#include "../../gpu_state/common.h"

#include "rn_light.h"
#include "rn_material.h"
#include "rn_pass.h"
#include "rn_prim.h"

void
gkRenderPass(GkScene     * __restrict scene,
             GkModelInst * __restrict modelInst,
             GkPrimitive * __restrict prim,
             GkMaterial  * __restrict material,
             GkPass      * __restrict pass) {
  GkContext   *ctx;
  GkProgram   *prog;
  GkSceneImpl *sceneImpl;

  if (!pass || !prim || !scene || !(prog = pass->prog))
    return;

  sceneImpl = (GkSceneImpl *)scene;
  ctx       = gkContextOf(scene);

  if (ctx->currState->prog != prog)
    gkUseProgram(ctx, prog);
  
  if (prog->lastMaterial != material && !pass->noMaterials)
    gkUniformMaterial(ctx, prog, material);

  gkUniformTransform(prog, modelInst->trans, scene->camera);
  
  if (!pass->noLights) {
    switch (sceneImpl->rpath) {
      case GK_RNPATH_MODEL_PERLIGHT:
        if (!material->transparent)
          gkRenderPrimPerLight(scene, prim, prog);
        else
          gkRenderTranspPrimPerLight(scene, prim, prog);
        break;
      case GK_RNPATH_SCENE_PERLIGHT:
        gkRenderPrimForLight(scene, modelInst, prim, prog);
        break;
      default: break;
    }
  } else {
    gkRenderPrim(scene, prim);
  }
}
