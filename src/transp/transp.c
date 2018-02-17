/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
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

  sceneImpl->renderFunc = gk__transp_rnfn;
  scene->flags         |= GK_SCENEF_TRANSP;
}

GK_EXPORT
void
gkDisableTransp(GkScene * __restrict scene) {
  GkSceneImpl *sceneImpl;

  sceneImpl = (GkSceneImpl *)scene;

  if (!sceneImpl->transp)
    gk__transp_intfn(scene);

  sceneImpl->renderFunc = NULL;
  scene->flags         &= ~GK_SCENEF_TRANSP;
}
