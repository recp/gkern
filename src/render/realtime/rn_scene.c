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
#include "../../default/transform.h"
#include "../../../include/gk/prims/cube.h"
#include "../../../include/gk/time.h"
#include "../../../include/gk/pass.h"
#include "../../default/def_light.h"
#include "../../../include/gk/gpu_state.h"
#include "../../../include/gk/clear.h"
#include "../../bbox/scene_bbox.h"
#include "rn_prim.h"
#include "rn_animator.h"

#include <tm/tm.h>

static
void
gkDefRenderFunc(GkScene * scene) ;

_gk_hide
void
gkPerModelInstTask(GkScene   * __restrict scene,
                   FListItem * __restrict iter);

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

  if ((scene->flags & GK_SCENEF_DRAW_BONES))
    gkDrawBones(scene);
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
      glm_vec3_rotate_m4(scene->camera->world,
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

  glm_aabb_invalidate(scene->bbox);
  if (!GK_FLG(scene->flags, GK_SCENEF_PREPARED))
    gkPrepareScene(scene);

  /* run animations */
  gkRunAnim(sceneImpl);

  /* no camera, create default one! */
  if (!scene->camera) {
    GkCamera *cam;
    cam = gkMakeCameraForScene(scene);
    gkSetCamera(scene, cam);
    gkCacheTransformsFor(scene, cam);
  }

  gkBindOutput(scene, scene->finalOutput);

  if (!sceneImpl->clearPipeline) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  } else {
    gkClearEffect(scene, sceneImpl->clearPipeline);
    glClear(GL_DEPTH_BUFFER_BIT);
  }

  /* todo: use frustum culler here */
  if (!(scene->trans->flags & GK_TRANSF_WORLD_ISVALID))
    gkApplyTransform(scene, scene->rootNode);

  if ((scene->camera->flags & GK_UPDT_VIEWPROJ))
    gkApplyView(scene, scene->rootNode);

  /* frustum culling */
  gkCullFrustum(scene, scene->camera);
  
  /* this can be combined with CullFrustum but it easy to magane in this way */
  gkPerModelInstTask(scene, scene->camera->frustum.modelInsList);

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

_gk_hide
void
gkPerModelInstTask(GkScene   * __restrict scene,
                   FListItem * __restrict iter) {
  GkGeometryInst  *geomInst;
  GkInstanceMorph *morpher;

  while (iter) {
    geomInst = iter->data;
    
    /* 1. Update Morph Weights */
    if ((morpher = geomInst->morpher)) {
      gkUniformTargetWeights(scene,
                             geomInst,
                             morpher->overrideWeights,
                             morpher->overrideWeightsCount);
    }
    
    iter = iter->next;
  }
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
