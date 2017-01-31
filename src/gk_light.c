/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk.h"
#include "gk_uniform.h"
#include <string.h>

void
gkUniformLight(struct GkScene * __restrict scene,
               GkLight        * __restrict light) {
  GkNode        *node;
  GkFinalMatrix *fmat;
  vec4  amb;
  char  buf[256];
  GLint loc;
  GLint prog;
  GLint enabled, isLocal, isSpot;
  GLint index;

  if (light->index == -1) {
    light->index = index = scene->lastLightIndex++;
  } else {
    index = light->index;
  }

  enabled = light->enabled;
  node    = light->node;
  fmat    = node->matrix->fmat;

  /* TODO: read uniform structure/names from options */
  strcpy(buf, "lights");
  sprintf(buf + strlen("lights"), "[%d].", index);

  prog = node->pinfo->prog;

  /* TODO: default ambient of light source */
  glm_vec4_dup((vec4){0.0, 0.0, 0.0, 1.0}, amb);

  switch (light->type) {
    case GK_LIGHT_TYPE_SPOT: {
      GkSpotLight *spot;
      vec3 dir;

      spot = (GkSpotLight *)light;
      isLocal = isSpot  = 1;

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

      /* cone direction */
      glm_vec_rotate_m4(fmat->cmv,
                        spot->direction,
                        dir);

      loc = gkGetUniformLoc(prog, buf, "direction");
      glUniform3fv(loc, 1, dir);
      break;
    }

    case GK_LIGHT_TYPE_POINT: {
      GkPointLight *point;

      point = (GkPointLight *)light;
      isLocal = 1;
      isSpot  = 0;

      loc = gkGetUniformLoc(prog, buf, "constAttn");
      glUniform1f(loc, point->constAttn);

      loc = gkGetUniformLoc(prog, buf, "linAttn");
      glUniform1f(loc, point->linearAttn);

      loc = gkGetUniformLoc(prog, buf, "quadAttn");
      glUniform1f(loc, point->quadAttn);
      break;
    }
    case GK_LIGHT_TYPE_DIRECTIONAL: {
      isLocal = isSpot = 0;
      break;
    }
    case GK_LIGHT_TYPE_AMBIENT: {
      isLocal = isSpot = 0;
      break;
    }
    default:
      printf("TODO: custom lights currently are unsupported\n");
      return;
  }

  loc = gkGetUniformLoc(prog, buf, "ambient");
  glUniform4fv(loc, 1, amb);

  loc = gkGetUniformLoc(prog, buf, "enabled");
  glUniform1i(loc, enabled);

  loc = gkGetUniformLoc(prog, buf, "isSpot");
  glUniform1i(loc, isSpot);

  loc = gkGetUniformLoc(prog, buf, "isLocal");
  glUniform1i(loc, isLocal);

  loc = gkGetUniformLoc(prog, buf, "color");
  glUniform4fv(loc, 1, light->color.vec);

  loc = gkGetUniformLoc(prog, buf, "position");

  /* position must be in view space */
  glUniform3fv(loc, 1, fmat->cmv[3]);
  light->isvalid = 1;

  loc = glGetUniformLocation(prog, "lightCount");
  glUniform1i(loc, scene->lightCount);
}

void
gkUniformLights(struct GkScene * __restrict scene) {
  GkLight *light;

  light = (GkLight *)scene->lights;
  while (light) {
    if (!light->isvalid)
      gkUniformLight(scene, light);

    light = (GkLight *)light->ref.next;
  }

  scene->lightsAreValid = 1;
}

