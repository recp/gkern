/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../../include/gk.h"
#include "../../default/gk_transform.h"

void
gkRenderScene(GkScene * scene) {
  GkMatrix *trans;

#ifdef DEBUG
  assert(scene->pinfo || "set default program / shader params");
#else
  /* there is no program! */
  return;
#endif

  if (gkCurrentProgram() != scene->pinfo->prog) {
    glUseProgram(scene->pinfo->prog);
    scene->currentProgram = scene->pinfo->prog;
  }

  if (scene->trans)
    trans = scene->trans;
  else
    trans = gk_def_idmat();

  if (!scene->lightsAreValid) {
    gkPrepNode(scene,
               scene->rootNode,
               trans,
               scene->pinfo);

    gkUniformLights(scene);
  }

  gkRenderNode(scene,
               scene->rootNode,
               trans,
               scene->pinfo);

  trans->cmatIsValid = 1;
  scene->pvIsValid   = 1;
  scene->vIsValid    = 1;
}
