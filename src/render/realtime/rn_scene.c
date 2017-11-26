/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../common.h"
#include "../../../include/gk/gk.h"
#include "../../default/transform.h"
#include "../../../include/gk/prims/cube.h"
#include "../../../include/gk/time.h"
#include "../../../include/gk/pass.h"
#include "../../default/def_light.h"
#include "../../../include/gk/gpu_state.h"

void
gkRenderScene(GkScene * scene) {
  double start, end;

  start = gkGetTime();

  if (!scene
      || ((scene->flags & GK_SCENEF_ONCE)
          && !(scene->flags & GK_SCENEF_NEEDS_RENDER)
          && !(scene->camera->flags & GK_UPDT_VIEWPROJ))
      || scene->flags & GK_SCENEF_RENDERING)
  return;

  scene->flags &= ~GK_SCENEF_RENDERED;
  scene->flags |= GK_SCENEF_RENDERING;

  if (!GK_FLG(scene->flags, GK_SCENEF_PREPARED)) {
    if (!scene->finalOutput)
      scene->finalOutput = gkDefaultRenderOut();

    if (!scene->trans)
      scene->trans = gk_def_idmat();

    if (!scene->_priv.rp) {
      scene->_priv.rp    = gkModelPerLightRenderPath;
      scene->_priv.rpath = GK_RNPATH_MODEL_PERLIGHT;
    }

    scene->flags |= GK_SCENEF_PREPARED;
  }

  gkBindPassOut(scene, scene->finalOutput);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* for collect all lights once */
  gkPrepNode(scene, scene->rootNode, scene->trans);

  scene->trans->flags  |= GK_TRANSF_WORLD_ISVALID;
  scene->camera->flags &= ~GK_UPDT_VIEWPROJ;

  scene->_priv.rp(scene);

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

GK_EXPORT
void
gkScenePerLightRenderPath(GkScene * __restrict scene) {
  GkLight     *light, *firstLight;
  GkTransform *trans;
  GkNode      *rootNode;

  /* default sun light */
  if (!(light = (GkLight *)scene->lights)) {
    light             = gk_def_lights();
    light->isvalid    = false;
    scene->lightCount = 1;
    scene->lights     = (GkLightRef *)light;
  }

  trans      = scene->trans;
  rootNode   = scene->rootNode;
  firstLight = light;

  do {
    scene->_priv.forLight = light;

    if (light != firstLight) {
      glDepthFunc(GL_EQUAL);
      glEnable(GL_BLEND);
      glBlendEquation(GL_FUNC_ADD);
      glBlendFunc(GL_ONE, GL_ONE);
    }

    if (scene->flags & GK_SCENEF_SHADOWS) {
      gkRenderShadows(scene, light);
    }

    //    gkRenderShadowMapTo(scene, scene->finalOutput);

    gkRenderNode(scene, rootNode, trans);
  } while ((light = (GkLight *)light->ref.next));
}

GK_EXPORT
void
gkModelPerLightRenderPath(GkScene * __restrict scene) {
  gkRenderNode(scene, scene->rootNode, scene->trans);
}
