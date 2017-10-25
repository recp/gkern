/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk/gk.h"
#include "program/uniform_cache.h"
#include "default/def_light.h"
#include "matrix.h"
#include <string.h>

void
gkUniformLight(struct GkScene * __restrict scene,
               GkLight        * __restrict light,
               GkProgInfo     * __restrict pinfo,
               mat4                        transView) {
  vec4  amb, dir;
  char  buf[256];
  GLint loc;
  GLint enabled;
  GLint index;

  if (light->index == -1)
    light->index = index = scene->lastLightIndex++;
  else
    index = light->index;

  enabled = light->enabled;

  /* TODO: read uniform structure/names from options */
  strcpy(buf, "lights");
  sprintf(buf + strlen("lights"), "[%d].", index);

  /* TODO: default ambient of light source */
  glm_vec4_copy((vec4){0.0, 0.0, 0.0, 1.0}, amb);

  switch (light->type) {
    case GK_LIGHT_TYPE_SPOT: {
      GkSpotLight *spot;

      spot = (GkSpotLight *)light;

      loc = gkUniformLocBuff(pinfo, "cutoffCos", buf);
      glUniform1f(loc, spot->cutoffCosine);

      loc = gkUniformLocBuff(pinfo, "cutoffExp", buf);
      glUniform1f(loc, spot->cutoffExp);

      loc = gkUniformLocBuff(pinfo, "constAttn", buf);
      glUniform1f(loc, spot->constAttn);

      loc = gkUniformLocBuff(pinfo, "linAttn", buf);
      glUniform1f(loc, spot->linearAttn);

      loc = gkUniformLocBuff(pinfo, "quadAttn", buf);
      glUniform1f(loc, spot->quadAttn);
      break;
    }

    case GK_LIGHT_TYPE_POINT: {
      GkPointLight *point;

      point = (GkPointLight *)light;

      loc = gkUniformLocBuff(pinfo, "constAttn", buf);
      glUniform1f(loc, point->constAttn);

      loc = gkUniformLocBuff(pinfo, "linAttn", buf);
      glUniform1f(loc, point->linearAttn);

      loc = gkUniformLocBuff(pinfo, "quadAttn", buf);
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

  loc = gkUniformLocBuff(pinfo, "enabled", buf);
  glUniform1i(loc, enabled);

  loc = gkUniformLocBuff(pinfo, "type", buf);
  glUniform1ui(loc, light->type);

  loc = gkUniformLocBuff(pinfo, "ambient", buf);
  glUniform4fv(loc, 1, amb);

  loc = gkUniformLocBuff(pinfo, "color", buf);
  glUniform4fv(loc, 1, light->color.vec);

  loc = gkUniformLocBuff(pinfo, "position", buf);

  /* position must be in view space */
  glUniform3fv(loc, 1, transView[3]);

  /* light/cone direction */
  glm_vec_rotate_m4(transView,
                    light->direction,
                    dir);

  loc = gkUniformLocBuff(pinfo, "direction", buf);
  glUniform3fv(loc, 1, dir);

  loc = gkUniformLocBuff(pinfo, "lightCount", buf);
  glUniform1i(loc, scene->lightCount);

  light->isvalid = 1;
}

void
gkUniformLights(struct GkScene * __restrict scene,
                GkProgInfo     * __restrict pinfo) {
  GkLight *light;

  light = (GkLight *)scene->lights;
  if (!light) {
    light             = gk_def_lights();
    light->isvalid    = false;
    scene->lightCount = 1;
  }

  while (light) {
    if (!light->isvalid) {
      if (light->node) {
        GkNode           *node;
        GkFinalTransform *ftr;

        node = light->node;
        ftr  = node->trans->ftr;

        gkUniformLight(scene,
                       light,
                       pinfo,
                       ftr->mv);
      } else {
        gkUniformLight(scene,
                       light,
                       pinfo,
                       GLM_MAT4_IDENTITY);
      }
    }

    light = (GkLight *)light->ref.next;
  }

  pinfo->updtLights = 0;
}
