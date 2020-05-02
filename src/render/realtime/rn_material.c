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

#include "../../state/gpu.h"
#include "../../default/def_effect.h"

#include "rn_material.h"
#include "rn_pass.h"
#include "rn_light.h"
#include "rn_prim.h"

GkMaterial*
gkMaterialFor(GkScene     *scene,
              GkModelInst *modelInst,
              GkPrimInst  *primInst) {
  GkModel     *model;
  GkSceneImpl *sceneImpl;
  GkMaterial  *material;

  material  = NULL;
  model     = modelInst->model;
  sceneImpl = (GkSceneImpl *)scene;

  if (sceneImpl->overrideMaterial)
    return sceneImpl->overrideMaterial;

  if (!(material = primInst->material))
    material = primInst->prim->material;

  if (!material && !(material = modelInst->material))
    material = model->material;

  if (!material)
    material = gk_def_material();
  
  primInst->activeMaterial = material;

  return material;
}

void
gkApplyMaterial(GkScene    * __restrict scene,
                GkPrimInst * __restrict primInst) {
  GkSceneImpl *sceneImpl;
  GkPass      *pass;
  GkMaterial  *material;

  sceneImpl = (GkSceneImpl *)scene;
  if (sceneImpl->overridePass) {
    pass = sceneImpl->overridePass;
    goto apply;
  }

  material = primInst->activeMaterial;

  /*
  if (!(pass = material->technique->pass)
       && !(material->technique->pass =
            pass = gkGetOrCreatPass(scene,
                                    sceneImpl->forLight,
                                    primInst,
                                    material)))
  */

  /* TODO: CACHE generated program */
  if (!(pass = gkGetOrCreatPass(scene,
                                sceneImpl->forLight,
                                primInst,
                                material)))
    return;

apply:
  while (pass) {
    gkRenderPass(scene, primInst, pass);
    pass = pass->next;
  }
}
