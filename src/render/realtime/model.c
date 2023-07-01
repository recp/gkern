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
#include "../../../include/gk/gk.h"
#include "../../../include/gk/material.h"
#include "../../../include/gk/prims/cube.h"
#include "../../default/def_effect.h"
#include "../../default/def_light.h"
#include "../../state/gpu.h"

#include "light.h"
#include "material.h"
#include "transp.h"
#include "pass.h"
#include "prim.h"

#include <string.h>

/*

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

    if ((camImpl->transfSlot != (1 << 30)
        && (updt || camImpl->pub.flags & GK_UPDT_VIEWPROJ)))
      gkCalcFinalTransf(scene, &camImpl->pub, tr);

    camItem = camItem->next;
  }

  if(updt && tr != ptr)
    tr->flags |= GK_TRANSF_WORLD_ISVALID;

  if (modelInst->model->bbox && (updt || !modelInst->bbox)) {
    if (!modelInst->bbox) {
      GkBBox *bbox;

      bbox = malloc(sizeof(*bbox));
      memcpy(bbox, modelInst->model->bbox, sizeof(*bbox));

      modelInst->bbox = bbox;
    }
    gkTransformAABB(tr, modelInst->bbox);
  }

  if (!modelInst->activeMaterial)
    return;

  gkPrepMaterial(scene, modelInst);
}

void
gkRenderModel(GkScene     *scene,
              GkModelInst *modelInst) {
  GkModel     *model;
  GkPrimitive *primi;

  model = modelInst->model;

  / * pre events * /
  if (model->events && model->events->onDraw)
    model->events->onDraw(model, NULL, false);

  / * render * /
  primi = model->prim;
  while (primi) {
    / * dont render transparent primitives at first * /
//    if (gkPrimIsTransparent(modelInst, primi))
//      goto cont;

    glBindVertexArray(primi->vao);
    gkApplyMaterials(scene, modelInst, primi);

  cont:
    primi = primi->next;
  }

  / * reset the state * /
  glBindVertexArray(0);

  if ((model->flags & GK_MODEL_FLAGS_DRAW_BBOX) && model->bbox)
    gkDrawBBox(scene,
               modelInst->bbox,
               modelInst->trans->world);

  / * post events * /
  if (model->events && model->events->onDraw)
    model->events->onDraw(model, NULL, true);
}


void
gkRnModelNoMatOPass(GkScene     *scene,
                    GkModelInst *modelInst) {
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

  / * render * /
  primi = model->prim;
  while (primi) {
    glBindVertexArray(primi->vao);

    gkUniformTransform(prog, modelInst->trans, cam);
    gkRenderPrim(scene, primi);

    primi = primi->next;
  }

  / * reset the state * /
  glBindVertexArray(0);
}

*/
