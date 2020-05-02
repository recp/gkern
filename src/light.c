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

#include "common.h"
#include "types/impl_transform.h"
#include "../include/gk/gk.h"
#include "program/uniform_cache.h"
#include "default/def_light.h"
#include <string.h>

void
gkUniformLight(struct GkScene * __restrict scene,
               GkLight        * __restrict light,
               GkPipeline     * __restrict prog,
               mat4                        transView) {
  GkSceneImpl *sceneImpl;
  vec4         amb, dir;
  char         buf[256];
  GLint        loc;
  GLint        enabled;
  GLint        index;

  sceneImpl = (GkSceneImpl *)scene;
  if (light->index == -1)
    light->index = index = sceneImpl->pub.lastLightIndex++;
  else
    index = light->index;

  enabled = !(light->flags & GK_LIGHTF_DISABLED);

  /* TODO: read uniform structure/names from options */
  strcpy(buf, "lights");
  sprintf(buf + strlen("lights"), "[%d].", index);

  /* TODO: default ambient of light source */
  glm_vec4_copy((vec4){0.0, 0.0, 0.0, 1.0}, amb);

  switch (light->type) {
    case GK_LIGHT_TYPE_SPOT: {
      GkSpotLight *spot;

      spot = (GkSpotLight *)light;

      loc = gkUniformLocBuff(prog, "cutoffCos", buf);
      glUniform1f(loc, spot->cutoffCosine);

      loc = gkUniformLocBuff(prog, "cutoffExp", buf);
      glUniform1f(loc, spot->cutoffExp);

      loc = gkUniformLocBuff(prog, "constAttn", buf);
      glUniform1f(loc, spot->constAttn);

      loc = gkUniformLocBuff(prog, "linAttn", buf);
      glUniform1f(loc, spot->linearAttn);

      loc = gkUniformLocBuff(prog, "quadAttn", buf);
      glUniform1f(loc, spot->quadAttn);
      break;
    }

    case GK_LIGHT_TYPE_POINT: {
      GkPointLight *point;

      point = (GkPointLight *)light;

      loc = gkUniformLocBuff(prog, "constAttn", buf);
      glUniform1f(loc, point->constAttn);

      loc = gkUniformLocBuff(prog, "linAttn", buf);
      glUniform1f(loc, point->linearAttn);

      loc = gkUniformLocBuff(prog, "quadAttn", buf);
      glUniform1f(loc, point->quadAttn);
      break;
    }
    case GK_LIGHT_TYPE_DIRECTIONAL:
    case GK_LIGHT_TYPE_AMBIENT:
      break;
    default:
      printf("TODO: custom lights currently are unsupported\n");
      return;
  }

  loc = gkUniformLocBuff(prog, "enabled", buf);
  glUniform1i(loc, enabled);

  loc = gkUniformLocBuff(prog, "type", buf);
  glUniform1ui(loc, light->type);

  loc = gkUniformLocBuff(prog, "ambient", buf);
  glUniform4fv(loc, 1, amb);

  loc = gkUniformLocBuff(prog, "color", buf);
  glUniform4fv(loc, 1, light->color.vec);

  loc = gkUniformLocBuff(prog, "position", buf);

  /* position must be in view space */
  glUniform3fv(loc, 1, transView[3]);

  /* light/cone direction */
  glm_vec3_rotate_m4(transView,
                    light->dir,
                    dir);

  loc = gkUniformLocBuff(prog, "direction", buf);
  glUniform3fv(loc, 1, dir);

  loc = gkUniformLocBuff(prog, "lightCount", buf);
  glUniform1i(loc, sceneImpl->pub.lightCount);

  light->isvalid = 1;
}

void
gkUniformLights(struct GkScene * __restrict scene,
                GkPipeline      * __restrict prog) {
  GkSceneImpl *sceneImpl;
  GkLight     *light;
  mat4         identity = GLM_MAT4_IDENTITY_INIT;

  sceneImpl = (GkSceneImpl *)scene;
  light     = (GkLight *)sceneImpl->pub.lights;
  if (!light) {
    light                 = gk_def_lights();
    light->isvalid        = false;
    sceneImpl->pub.lightCount = 1;
  }

  while (light) {
    if (!light->isvalid) {
      if (light->node) {
        GkNode           *node;
        GkFinalTransform *ftr;

        node = light->node;
        if ((ftr = gkFinalTransform(node->trans, scene->camera)))
          gkUniformLight(scene, light, prog, ftr->mv);
        else
          gkUniformLight(scene, light, prog, identity);
      } else {
        gkUniformLight(scene, light, prog, GLM_MAT4_IDENTITY);
      }
    }

    light = (GkLight *)light->ref.next;
  }

  prog->updtLights = 0;
}
