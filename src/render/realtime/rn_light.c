/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../common.h"
#include "../../../include/gk/gk.h"
#include "../../../include/gk/material.h"
#include "../../../include/gk/prims/cube.h"
#include "../../default/def_effect.h"
#include "../../default/def_light.h"
#include "../../shadows/shadows.h"
#include "../../program/uniform_cache.h"

#include "rn_light.h"
#include "rn_pass.h"
#include "rn_prim.h"

extern void *GK_SHADOWS_HANDLE;
static mat4 gk__biasMatrix = {
  0.5f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.5f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.5f, 0.0f,
  0.5f, 0.5f, 0.5f, 1.0f
};

void
gkRenderPrimForLight(GkScene     * __restrict scene,
                     GkModelInst * __restrict modelInst,
                     GkPrimitive * __restrict prim,
                     GkProgram   * __restrict prog) {
  GkLight     *light;
  GkSceneImpl *sceneImpl;

  sceneImpl = (GkSceneImpl *)scene;
  light = sceneImpl->forLight;
  if (light != prog->lastLight)
    gkUniformSingleLight(scene, light, prog);
  else if (light->flags & GK_LIGHTF_TRANSFORMED)
    gkApplyTransformToLight(scene, light, prog);

  if (GK_FLG(scene->flags, GK_SCENEF_SHADOWS)) {
    mat4         lightMVP;
    GkShadowMap *sm;
    int32_t      smUnit;

    sm     = sceneImpl->shadows;
    smUnit = flist_indexof(gkContextOf(scene)->samplers,
                           GK_SHADOWS_HANDLE);

    gkBindDepthTexTo(scene,
                     sm->shadowPass,
                     prog,
                     smUnit,
                     "uShadowMap");

    glm_mat4_mul(sm->viewProj[0], modelInst->trans->world, lightMVP);

    gkUniformMat4(gkUniformLoc(prog, "uLightMVP"), lightMVP);
  }

  gkRenderPrim(scene, prim);
}

void
gkRenderPrimPerLight(GkScene     * __restrict scene,
                     GkPrimitive * __restrict prim,
                     GkProgram   * __restrict prog) {
  GkSceneImpl *sceneImpl;
  GkLight     *firstLight, *light;

  sceneImpl = (GkSceneImpl *)scene;
  light     = (GkLight *)sceneImpl->pub.lights;
  if (!light) {
    light                 = gk_def_lights();
    light->isvalid        = false;
    sceneImpl->pub.lightCount = 1;
  }

  firstLight = light;

  do {
    if (light != firstLight) {
      glDepthFunc(GL_EQUAL);
      glEnable(GL_BLEND);
      glBlendEquation(GL_FUNC_ADD);
      glBlendFunc(GL_ONE, GL_ONE);
    }

    if (light != prog->lastLight)
      gkUniformSingleLight(scene, light, prog);
    else if (light->flags & GK_LIGHTF_TRANSFORMED)
      gkApplyTransformToLight(scene, light, prog);

    gkRenderPrim(scene, prim);

    light = (GkLight *)light->ref.next;
  } while (light);

  glDepthFunc(GL_LESS);
  glDisable(GL_BLEND);
}

void
gkRenderTranspPrimPerLight(GkScene     * __restrict scene,
                           GkPrimitive * __restrict prim,
                           GkProgram   * __restrict prog) {
  GkSceneImpl *sceneImpl;
  GkLight     *firstLight, *light;

  sceneImpl = (GkSceneImpl *)scene;
  light     = (GkLight *)sceneImpl->pub.lights;
  if (!light) {
    light                 = gk_def_lights();
    light->isvalid        = false;
    sceneImpl->pub.lightCount = 1;
  }

  firstLight = light;

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  do {
    if (light != firstLight) {
      glDepthFunc(GL_LEQUAL);
      glBlendEquation(GL_FUNC_ADD);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    }

    if (light != prog->lastLight)
      gkUniformSingleLight(scene, light, prog);
    else if (light->flags & GK_LIGHTF_TRANSFORMED)
      gkApplyTransformToLight(scene, light, prog);

    gkRenderPrim(scene, prim);

    light = (GkLight *)light->ref.next;
  } while (light);

  glDepthFunc(GL_LESS);
  glDisable(GL_BLEND);
}
