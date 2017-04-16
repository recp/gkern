/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../../include/gk.h"
#include "../../default/gk_transform.h"
#include "../../../include/prims/gk-cube.h"

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

  if (scene->flags & GK_SCENEF_UPDT_LIGHTS) {
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
  scene->flags &= ~(GK_SCENEF_UPDT_VIEWPROJ | GK_SCENEF_UPDT_VIEW);

  if ((scene->flags & GK_SCENEF_DRAW_BBOX)
      && scene->bbox)
    gkDrawBBox(scene,
               scene->rootNode->matrix->cmat,
               scene->bbox->min,
               scene->bbox->max);
}
