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
#include "rn_prim.h"

static
void
gkPrepareScene(GkScene * scene);

static
void
gkPrepareScene(GkScene * scene) {
  GkSceneImpl *sceneImpl;

  sceneImpl = (GkSceneImpl *)scene;
  if (!scene->finalOutput)
    scene->finalOutput = gkDefaultRenderOut();

  if (!scene->trans)
    scene->trans = gk_def_idmat();

  if (!sceneImpl->rp) {
    sceneImpl->rp    = gkModelPerLightRenderPath;
    sceneImpl->rpath = GK_RNPATH_MODEL_PERLIGHT;
  }

  /* default sun light */
  if (!sceneImpl->pub.lights) {
    GkLight *light;
    light                     = gk_def_lights();
    light->isvalid            = false;
    sceneImpl->pub.lightCount = 1;
    sceneImpl->pub.lights     = (GkLightRef *)light;
  }

  scene->flags |= GK_SCENEF_PREPARED;
}

void
gkRenderScene(GkScene * scene) {
  GkSceneImpl *sceneImpl;
  double       start, end;

  start = gkGetTime();

  if (!scene
      || ((scene->flags & GK_SCENEF_ONCE)
          && !(scene->flags & GK_SCENEF_NEEDS_RENDER)
          && !(scene->camera->flags & GK_UPDT_VIEWPROJ))
      || scene->flags & GK_SCENEF_RENDERING)
  return;

  sceneImpl     = (GkSceneImpl *)scene;
  scene->flags &= ~GK_SCENEF_RENDERED;
  scene->flags |= GK_SCENEF_RENDERING;

  if (!GK_FLG(scene->flags, GK_SCENEF_PREPARED))
    gkPrepareScene(scene);

  gkBindPassOut(scene, scene->finalOutput);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* todo: use frustum culler here */
  if (!(scene->trans->flags & GK_TRANSF_WORLD_ISVALID)) {
    gkApplyTransform(scene, scene->rootNode);
    gkTransformAABB(scene->trans, scene->bbox);
  }

  if ((scene->camera->flags & GK_UPDT_VIEW))
    gkApplyView(scene, scene->rootNode);

  /* frustum culling */
  gkCullFrustum(scene, scene->camera);

  scene->trans->flags  |= GK_TRANSF_WORLD_ISVALID;
  scene->camera->flags &= ~GK_UPDT_VIEWPROJ;

  sceneImpl->rp(scene);

  if ((scene->flags & GK_SCENEF_DRAW_BBOX)
      && scene->bbox)
    gkDrawBBox(scene,
               scene->bbox,
               scene->rootNode->trans->world);

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
  GkSceneImpl *sceneImpl;
  GkFrustum   *frustum;
  GkLight     *light, *firstLight;
  GkPrimInst  **prims;
  size_t       i, primc;

  sceneImpl  = (GkSceneImpl *)scene;
  light      = (GkLight *)sceneImpl->pub.lights;
  firstLight = light;
  frustum    = &scene->camera->frustum;
  
  do {
    sceneImpl->forLight = light;

    if (light != firstLight) {
      glDepthFunc(GL_EQUAL);
      glEnable(GL_BLEND);
      glBlendEquation(GL_FUNC_ADD);
      glBlendFunc(GL_ONE, GL_ONE);
    }

    if (scene->flags & GK_SCENEF_SHADOWS)
      gkRenderShadows(scene, light);

    /* opaque objects */
    primc = frustum->opaque->count;
    prims = frustum->opaque->items;
    for (i = 0; i < primc; i++) {
      if (!scene->renderPrimFunc)
        gkRenderPrimInst(scene, prims[i]);
      else
        scene->renderPrimFunc(scene, prims[i]);
    }
    
    /* transparent objects */

  } while ((light = (GkLight *)light->ref.next));
}

GK_EXPORT
void
gkModelPerLightRenderPath(GkScene * __restrict scene) {
  GkFrustum    *frustum;
  GkPrimInst  **prims;
  size_t        i, primc;
  
  frustum = &scene->camera->frustum;

  /* opaque objects */
  primc = frustum->opaque->count;
  prims = frustum->opaque->items;
  for (i = 0; i < primc; i++) {
    if (!scene->renderPrimFunc)
      gkRenderPrimInst(scene, prims[i]);
    else
      scene->renderPrimFunc(scene, prims[i]);
  }
  
  /* transparent objects */
}
