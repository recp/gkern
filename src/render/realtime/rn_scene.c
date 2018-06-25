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
#include "../../bbox/scene_bbox.h"
#include "rn_prim.h"
#include "rn_animator.h"

#include <tm/tm.h>

static
void
gkDefRenderFunc(GkScene * scene) ;

static
void
gkDefRenderFunc(GkScene * scene) {
  GkFrustum *frustum;

  frustum = &scene->camera->frustum;

  if ((scene->flags & GK_SCENEF_DRAW_BBOX))
    gkDrawBBox(scene,
               scene->bbox,
               scene->rootNode->trans->world);

  gkRenderPrims(scene, frustum->opaque);
  gkRenderPrims(scene, frustum->transp);
}

GK_EXPORT
void
gkPrepareScene(GkScene * __restrict scene) {
  GkSceneImpl *sceneImpl;

  sceneImpl = (GkSceneImpl *)scene;
  if (!scene->finalOutput)
    scene->finalOutput = gkDefaultRenderOut();

  if (!scene->trans)
    scene->trans = gk_def_idmat();

  if (!sceneImpl->rp) {
    sceneImpl->rp    = gkScenePerLightRenderPath;
    sceneImpl->rpath = GK_RNPATH_SCENE_PERLIGHT;
  }

  /* default sun light */
  if (!sceneImpl->pub.lights) {
    GkLight *light;
    light                     = gk_def_lights();
    light->isvalid            = false;
    sceneImpl->pub.lightCount = 1;
    sceneImpl->pub.lights     = (GkLightRef *)light;

    if (scene->camera) {
      glm_vec_rotate_m4(scene->camera->world,
                        light->defdir,
                        light->dir);
    }
  }

  if (!(scene->trans->flags & GK_TRANSF_WORLD_ISVALID))
    gkApplyTransform(scene, scene->rootNode);

  scene->flags |= GK_SCENEF_PREPARED;
}

void
gkRenderScene(GkScene * scene) {
  GkSceneImpl *sceneImpl;

  sceneImpl = (GkSceneImpl *)scene;
  scene->startTime = tm_time();

  if (!scene
      || ((scene->flags & GK_SCENEF_ONCE)
          && !(scene->flags & GK_SCENEF_NEEDS_RENDER)
          && !(scene->camera->flags & GK_UPDT_VIEWPROJ)
          && !sceneImpl->anims)
      || scene->flags & GK_SCENEF_RENDERING)
  return;

  scene->flags &= ~GK_SCENEF_RENDERED;
  scene->flags |= GK_SCENEF_RENDERING;

  /* run animations */
  gkRunAnim(sceneImpl);

  glm_aabb_invalidate(scene->bbox);

  if (!GK_FLG(scene->flags, GK_SCENEF_PREPARED))
    gkPrepareScene(scene);

  /* no camera, create default one! */
  if (!scene->camera) {
    GkCamera *cam;
    cam = gkMakeCameraForScene(scene);
    gkSetCamera(scene, cam);
    gkCacheTransformsFor(scene, cam);
  }

  gkBindOutput(scene, scene->finalOutput);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* todo: use frustum culler here */
  if (!(scene->trans->flags & GK_TRANSF_WORLD_ISVALID))
    gkApplyTransform(scene, scene->rootNode);

  if ((scene->camera->flags & GK_UPDT_VIEW))
    gkApplyView(scene, scene->rootNode);

  /* frustum culling */
  gkCullFrustum(scene, scene->camera);

  scene->trans->flags  |= GK_TRANSF_WORLD_ISVALID;
  scene->camera->flags &= ~GK_UPDT_VIEWPROJ;

  sceneImpl->rp(scene);

  scene->flags &= ~GK_SCENEF_UPDT_LIGHTS;
  scene->flags &= ~GK_SCENEF_NEEDS_RENDER;
  scene->flags &= ~GK_SCENEF_RENDERING;
  scene->flags |= GK_SCENEF_RENDERED;

  scene->endTime = tm_time();

  scene->fpsApprx = 1.0 / (scene->endTime - scene->startTime);
}

GK_EXPORT
void
gkScenePerLightRenderPath(GkScene * __restrict scene) {
  GkSceneImpl *sceneImpl;
  GkLight     *light, *firstLight;
  GkContext   *ctx;

  ctx        = gkContextOf(scene);
  sceneImpl  = (GkSceneImpl *)scene;
  light      = (GkLight *)sceneImpl->pub.lights;
  firstLight = light;

  if (sceneImpl->lightIterFunc) {
    sceneImpl->lightIterFunc(scene);
    return;
  }

  do {
    sceneImpl->forLight = light;

    if (light != firstLight) {
      gkDepthFunc(ctx, GL_EQUAL);
      gkEnableBlend(ctx);
      gkBlendEq(ctx, GL_FUNC_ADD);
      gkBlendFunc(ctx, GL_ONE, GL_ONE);
    }

    if (scene->flags & GK_SCENEF_SHADOWS)
      gkRenderShadows(scene, light);

    if (!sceneImpl->renderFunc)
      gkDefRenderFunc(scene);
    else
      sceneImpl->renderFunc(scene);
  } while ((light = (GkLight *)light->ref.next));

  gkDisableBlend(ctx);
  gkDepthFunc(ctx, GL_LESS);
}

GK_EXPORT
void
gkModelPerLightRenderPath(GkScene * __restrict scene) {
  GkSceneImpl *sceneImpl;

  sceneImpl = (GkSceneImpl *)scene;
  if (!sceneImpl->renderFunc)
    gkDefRenderFunc(scene);
  else
    sceneImpl->renderFunc(scene);
}
