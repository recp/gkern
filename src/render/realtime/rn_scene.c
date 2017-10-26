/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../../include/gk/gk.h"
#include "../../default/transform.h"
#include "../../../include/gk/prims/cube.h"
#include "../../../include/gk/time.h"
#include "../../../include/gk/pass.h"

void
gkRenderScene(GkScene * scene) {
  GkTransform *trans;
  double       start, end;

  start = gkGetTime();

  if (!scene
      || ((scene->flags & GK_SCENEF_ONCE)
          && !(scene->flags & GK_SCENEF_NEEDS_RENDER))
      || scene->flags & GK_SCENEF_RENDERING)
  return;

#ifdef DEBUG
  assert(scene->pinfo && "set default program / shader params");
#else
  /* there is no program! */
  if (!scene->pinfo)
    return;
#endif

  scene->flags &= ~GK_SCENEF_RENDERED;
  scene->flags |= GK_SCENEF_RENDERING;

  if (!scene->finalOutput)
    scene->finalOutput = gkDefaultRenderOut();

  gkBindPassOut(scene->finalOutput);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (scene->trans)
    trans = scene->trans;
  else
    trans = gk_def_idmat();

  /* for collect all lights once 
     TODO: optimize it
   */
  gkPrepNode(scene,
             scene->rootNode,
             trans);

  trans->flags |= GK_TRANSF_WORLD_ISVALID;
  scene->flags &= ~(GK_SCENEF_UPDT_VIEWPROJ);

  gkRenderNode(scene,
               scene->rootNode,
               trans);

  if ((scene->flags & GK_SCENEF_DRAW_BBOX)
      && scene->bbox)
    gkDrawBBox(scene,
               scene->rootNode->trans->world,
               scene->bbox->min,
               scene->bbox->max);

  scene->flags &= ~GK_SCENEF_UPDT_LIGHTS;
  scene->flags &= ~GK_SCENEF_NEEDS_RENDER;
  scene->flags &= ~GK_SCENEF_RENDERING;
  scene->flags |= GK_SCENEF_RENDERED;

  end = gkGetTime();

  scene->fpsApprx = 1.0 / (end - start);
}
