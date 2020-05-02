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

#include "../../include/gk/gk.h"
#include "../../include/gk/transparent.h"

#include "transp.h"
#include "builtin/oit/weighted_blended.h"
#include "builtin/phenomenological.h"

GkTranspTechnInitFunc   gk__transp_intfn = gkTranspWeightedBlendedInit;
GkTranspTechnRenderFunc gk__transp_rnfn  = gkTranspWeightedBlended;
GkTranspTechnType       gk__transp_techn = GK_TRANSP_WEIGHTED_BLENDED;

GK_EXPORT
void
gkSetTranspTechn(GkTranspTechnType techn) {
  switch (techn) {
    case GK_TRANSP_AUTO:
    case GK_TRANSP_WEIGHTED_BLENDED:
      gk__transp_intfn = gkTranspWeightedBlendedInit;
      gk__transp_rnfn  = gkTranspWeightedBlended;
      gk__transp_techn = GK_TRANSP_WEIGHTED_BLENDED;
      break;
    default:
      break;
  }
}

GK_EXPORT
GkTranspTechnType
gkTranspTechn() {
  return gk__transp_techn;
}

GK_EXPORT
void
gkEnableTransp(GkScene * __restrict scene) {
  GkSceneImpl *sceneImpl;

  sceneImpl = (GkSceneImpl *)scene;

  if (!sceneImpl->transp)
    gk__transp_intfn(scene);

  sceneImpl->lightIterFunc = gk__transp_rnfn;
  scene->flags            |= GK_SCENEF_TRANSP;

  sceneImpl->rpath = GK_RNPATH_SCENE_PERLIGHT;
  sceneImpl->rp    = gkScenePerLightRenderPath;
}

GK_EXPORT
void
gkDisableTransp(GkScene * __restrict scene) {
  GkSceneImpl *sceneImpl;

  sceneImpl = (GkSceneImpl *)scene;

  if (!sceneImpl->transp)
    gk__transp_intfn(scene);

  if (scene->flags & GK_SCENEF_SHADOWS) {
    sceneImpl->rpath = GK_RNPATH_SCENE_PERLIGHT;
    sceneImpl->rp    = gkScenePerLightRenderPath;
  } else {
    sceneImpl->rpath = GK_RNPATH_MODEL_PERLIGHT;
    sceneImpl->rp    = gkModelPerLightRenderPath;
  }

  sceneImpl->lightIterFunc = NULL;
  scene->flags            &= ~GK_SCENEF_TRANSP;
}

_gk_hide
GkRenderPathFn
gkTranpRenderFunc() {
  return gk__transp_rnfn;
}
