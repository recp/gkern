/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk.h"
#include <string.h>

/* default light direction */
static vec3 gkDefaultDir = {0, 0, -1};

GLint
gkGetUniformLoc(GLuint prog,
                char  * __restrict buf,
                char  * __restrict name) {
  char *pBuf;

  GLint loc;
  if (!buf)
    return glGetUniformLocation(prog, name);

  pBuf = strrchr(buf, '.') + 1;
  strcpy(pBuf, name);

  loc = glGetUniformLocation(prog, buf);
  return loc;
}

void
gkUniformLight(struct GkScene * __restrict scene,
               GkLight        * __restrict light) {
  GkNode *node;
  char    buf[256];
  GLint   loc;
  GLint   prog;
  GLint   enabled, isLocal, isSpot;
  GLint   index;

  if (light->index == -1) {
    light->index = index = scene->lastLightIndex++;
    scene->lightCount++;
  } else {
    index = light->index;
  }

  enabled = light->enabled;
  node    = light->node;

  /* TODO: read uniform structure/names from options */
  strcpy(buf, "lights");
  sprintf(buf + strlen("lights"), "[%d].", index);

  prog = node->pinfo->prog;

  vec3 amb = {0.5, 0.5, 0.5};

  switch (light->type) {
    case GK_LIGHT_TYPE_SPOT: {
      GkSpotLight *spot;
      vec3 dir;

      spot = (GkSpotLight *)light;
      isLocal = isSpot  = 1;

      loc = gkGetUniformLoc(prog, buf, "falloffCos");
      glUniform1f(loc, spot->falloffCosine);

      loc = gkGetUniformLoc(prog, buf, "falloffExp");
      glUniform1f(loc, spot->falloffExp);

      loc = gkGetUniformLoc(prog, buf, "constAttn");
      glUniform1f(loc, spot->constAttn);

      loc = gkGetUniformLoc(prog, buf, "linAttn");
      glUniform1f(loc, spot->linearAttn);

      loc = gkGetUniformLoc(prog, buf, "quadAttn");
      glUniform1f(loc, spot->quadAttn);

      /* cone direction */
      glm_vec_scale(node->matrix->fmat->cmv[3],
                    -1.0f,
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
  glUniform3fv(loc, 1, amb);

  loc = gkGetUniformLoc(prog, buf, "enabled");
  glUniform1i(loc, enabled);

  loc = gkGetUniformLoc(prog, buf, "isSpot");
  glUniform1i(loc, isSpot);

  loc = gkGetUniformLoc(prog, buf, "isLocal");
  glUniform1i(loc, isLocal);

  loc = gkGetUniformLoc(prog, buf, "color");
  glUniform3fv(loc, 1, light->color.vec);

  loc = gkGetUniformLoc(prog, buf, "position");

  glUniform3fv(loc,
               1,
               node->matrix->fmat->cmv[3]);
  light->isvalid = 1;
}

void
gkUniformLights(struct GkScene * __restrict scene) {
  GkLight *light;
  GLuint   loc;

  light = (GkLight *)scene->lights;
  while (light) {
    if (!light->isvalid)
      gkUniformLight(scene, light);

    light = (GkLight *)light->ref.next;
  }

  scene->lightsAreValid = 1;
}

