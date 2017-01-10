/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk.h"

static GkMatrix idmat = {
  1,
  1,
  NULL,
  GLM_MAT4_IDENTITY_INIT,
  GLM_MAT4_IDENTITY_INIT
};

void
gkRenderScene(GkScene * scene) {
#ifdef DEBUG
  assert(scene->pinfo || "set default program / shader params");
#endif

  gkRenderNode(scene,
               scene->rootNode,
               &idmat,
               scene->pinfo);

  scene->pvIsValid = 1;
}
