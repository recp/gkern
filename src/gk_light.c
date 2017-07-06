/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk/gk.h"
#include "gk_uniform.h"
#include "default/ak_def_light.h"
#include "gk_matrix.h"
#include <string.h>

void
gkUniformLight(struct GkScene * __restrict scene,
               GkLight        * __restrict light,
               GkProgInfo     * __restrict pinfo,
               mat4                        transView) {
  vec4  amb, dir;
  char  buf[256];
  GLint loc;
  GLint prog;
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

  prog = pinfo->prog;

  /* TODO: default ambient of light source */
  glm_vec4_copy((vec4){0.0, 0.0, 0.0, 1.0}, amb);

  switch (light->type) {
    case GK_LIGHT_TYPE_SPOT: {
      GkSpotLight *spot;

      spot = (GkSpotLight *)light;

      loc = gkGetUniformLoc(prog, buf, "cutoffCos");
      glUniform1f(loc, spot->cutoffCosine);

      loc = gkGetUniformLoc(prog, buf, "cutoffExp");
      glUniform1f(loc, spot->cutoffExp);

      loc = gkGetUniformLoc(prog, buf, "constAttn");
      glUniform1f(loc, spot->constAttn);

      loc = gkGetUniformLoc(prog, buf, "linAttn");
      glUniform1f(loc, spot->linearAttn);

      loc = gkGetUniformLoc(prog, buf, "quadAttn");
      glUniform1f(loc, spot->quadAttn);
      break;
    }

    case GK_LIGHT_TYPE_POINT: {
      GkPointLight *point;

      point = (GkPointLight *)light;

      loc = gkGetUniformLoc(prog, buf, "constAttn");
      glUniform1f(loc, point->constAttn);

      loc = gkGetUniformLoc(prog, buf, "linAttn");
      glUniform1f(loc, point->linearAttn);

      loc = gkGetUniformLoc(prog, buf, "quadAttn");
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

  loc = gkGetUniformLoc(prog, buf, "enabled");
  glUniform1i(loc, enabled);

  loc = gkGetUniformLoc(prog, buf, "type");
  glUniform1ui(loc, light->type);

  loc = gkGetUniformLoc(prog, buf, "ambient");
  glUniform4fv(loc, 1, amb);

  loc = gkGetUniformLoc(prog, buf, "color");
  glUniform4fv(loc, 1, light->color.vec);

  loc = gkGetUniformLoc(prog, buf, "position");

  /* position must be in view space */
  glUniform3fv(loc, 1, transView[3]);

  /* light/cone direction */
  glm_vec_rotate_m4(transView,
                    light->direction,
                    dir);

  loc = gkGetUniformLoc(prog, buf, "direction");
  glUniform3fv(loc, 1, dir);

  loc = glGetUniformLocation(prog, "lightCount");
  glUniform1i(loc, scene->lightCount);

  light->isvalid = 1;
}

void
gkUniformLightPos(GkNode * __restrict node) {
  GkFinalTransform *ftr;
  GkLight          *light;
  vec4              dir;
  char              buf[32];
  GLint             loc, prog;

  light = node->light;
  if (light->index == -1)
    return;

  ftr  = node->trans->ftr;
  prog = node->pinfo->prog;

  strcpy(buf, "lights");
  while (light) {
    sprintf(buf + strlen("lights"), "[%d].", light->index);
    loc = gkGetUniformLoc(prog, buf, "position");

    /* position must be in view space */
    glUniform3fv(loc, 1, ftr->mv[3]);

    /* light/cone direction */
    glm_vec_rotate_m4(ftr->mv, light->direction, dir);

    loc = gkGetUniformLoc(prog, buf, "direction");
    glUniform3fv(loc, 1, dir);

    light = light->next;
  }
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
