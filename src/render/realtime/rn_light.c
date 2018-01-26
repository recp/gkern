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
    GkShadowMap *sm;
    mat4        *shadowMVP;
    int32_t      smUnit, split, i;

    sm     = hash_get(sceneImpl->shadows, &light->type);
    smUnit = flist_indexof(gkContextOf(scene)->samplers,
                           GK_SHADOWS_HANDLE);

    if ((split = sm->splitc) < 1)
      split = 1;

    shadowMVP = alloca(sizeof(mat4) * split);

    if (gkShadowTechn() != GK_SHADOW_BASIC_SHADOWMAP) {
      gkBindDepthTexArrayTo(scene, sm->pass, prog, smUnit, "uShadMap");
      glUniform1fv(gkUniformLoc(prog, "uShadDist"),
                   split,
                   sm->distances);
    } else {
      gkBindDepthTexTo(scene, sm->pass, prog, smUnit, "uShadMap");
    }

    if (light->type != GK_LIGHT_TYPE_POINT) {
      for (i = 0; i < split; i++) {
        glm_mat4_mul(sm->viewProj[i],
                     modelInst->trans->world,
                     shadowMVP[i]);
      }

      glUniformMatrix4fv(gkUniformLoc(prog, "uShadMVP"),
                         split,
                         GL_FALSE,
                         shadowMVP[0][0]);
    } else {
      float n, f, nfsub, nf[2];

      n = sm->near;
      f = sm->far;

      nfsub = f - n;
      nf[0] = (f + n) / nfsub * 0.5f + 0.5f;
      nf[1] = (-2.0f * f * n) / nfsub * 0.5f;

      glUniform2f(gkUniformLoc(prog, "uFarNear"), nf[0], nf[1]);
      gkUniformMat4(gkUniformLoc(prog, "M"), modelInst->trans->world);
    }
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
