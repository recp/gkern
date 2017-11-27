/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../common.h"
#include "../../../include/gk/gk.h"
#include "../../../include/gk/material.h"
#include "../../../include/gk/prims/cube.h"
#include "../../default/def_effect.h"
#include "../../default/def_light.h"
#include "../../gpu_state/common.h"

#include "rn_light.h"
#include "rn_material.h"
#include "rn_pass.h"
#include "rn_prim.h"

void
gkPrepModel(GkScene     *scene,
            GkModelInst *modelInst,
            GkTransform *ptr) {
  GkSceneImpl  *sceneImpl;
  GkTransform  *tr;
  FListItem    *camItem;
  GkCameraImpl *camImpl;
  uint32_t      updt;

  sceneImpl = (GkSceneImpl *)scene;
  if (!(tr = modelInst->trans))
    modelInst->trans = tr = ptr;

  camItem = sceneImpl->transfCacheSlots->first;

  updt = !((ptr->flags & tr->flags) & GK_TRANSF_WORLD_ISVALID);
  if (updt && ptr != tr) {
    if (!GK_FLG(tr->flags, GK_TRANSF_LOCAL_ISVALID))
      gkTransformCombine(tr);

    glm_mat4_mul(ptr->world, tr->local, tr->world);
    tr->flags &= ~GK_TRANSF_WORLD_ISVALID;
  }

  while (camItem) {
    camImpl = camItem->data;

    if ((camImpl->transfSlot != (1 << 31)
        && (updt || camImpl->pub.flags & GK_UPDT_VIEWPROJ)))
      gkCalcFinalTransf(scene, &camImpl->pub, tr);

    camItem = camItem->next;
  }

  if(updt && tr != ptr)
    tr->flags |= GK_TRANSF_WORLD_ISVALID;

  if (!modelInst->activeMaterial)
    return;

  gkPrepMaterial(scene, modelInst);
}

void
gkRenderModel(GkScene     *scene,
              GkModelInst *modelInst,
              GkTransform *ptr) {
  GkModel     *model;
  GkPrimitive *primi;

  model = modelInst->model;

  /* pre events */
  if (model->events && model->events->onDraw)
    model->events->onDraw(model, NULL, false);

  /* render */
  primi = model->prim;
  while (primi) {
    glBindVertexArray(primi->vao);

    gkApplyMaterials(scene, modelInst, primi);

    primi = primi->next;
  }

  /* reset the state */
  glBindVertexArray(0);

  if ((model->flags & GK_MODEL_FLAGS_DRAW_BBOX) && model->bbox)
    gkDrawBBox(scene,
               modelInst->trans->world,
               model->bbox->min,
               model->bbox->max);

  /* post events */
  if (model->events && model->events->onDraw)
    model->events->onDraw(model, NULL, true);
}

void
gkRnModelNoMatOPass(GkScene     *scene,
                    GkModelInst *modelInst,
                    GkTransform *ptr) {
  GkSceneImpl *sceneImpl;
  GkContext   *ctx;
  GkModel     *model;
  GkPrimitive *primi;
  GkProgram   *prog;
  GkCamera    *cam;

  sceneImpl = (GkSceneImpl *)scene;
  if (!sceneImpl->overridePass
      || !(prog = sceneImpl->overridePass->prog))
    return;

  ctx = gkContextOf(scene);
  if (ctx->currState->prog != prog)
    gkUseProgram(ctx, prog);

  cam   = scene->camera;
  model = modelInst->model;

  /* render */
  primi = model->prim;
  while (primi) {
    glBindVertexArray(primi->vao);

    gkUniformTransform(prog, modelInst->trans, cam);
    gkRenderPrim(scene, primi);

    primi = primi->next;
  }

  /* reset the state */
  glBindVertexArray(0);
}
