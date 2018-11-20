/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "draw.h"
#include "../default/def_prog.h"
#include "../program/uniform_cache.h"
#include "../types/impl_scene.h"
#include "../state/gpu.h"
#include <limits.h>

GLuint gk__bone_vao = UINT_MAX;
GLuint gk__bone_ubo = UINT_MAX;

_gk_hide
void
gkDrawBonesInit(void);

_gk_hide
void
gkDrawBonesRelease(void);

_gk_hide
void
gkDrawBonesInit() {
  glGenVertexArrays(1, &gk__bone_vao);
  glBindVertexArray(gk__bone_vao);

  if (gk__bone_ubo == UINT_MAX) {
    size_t uboSize;

    uboSize = sizeof(mat4) * 255;  /* TODO: */

    glGenBuffers(1, &gk__bone_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, gk__bone_ubo);
    glBufferData(GL_UNIFORM_BUFFER,
                 uboSize,
                 NULL,
                 GL_DYNAMIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, gk__bone_ubo, 0, uboSize);
  }
}

GK_EXPORT
void
gkDrawBones(GkScene * __restrict scene) {
  GkContext        *ctx;
  GkSceneImpl      *sceneImpl;
  GkSkin           *skin;
  FListItem        *item;
  GkNode           *node;
  GkControllerInst *ctlrInst;
  GkModelInst      *modelInst;
  GkPipeline       *prog;

  ctx  = gkContextOf(scene);
  prog = gk_prog_drawbone();

  gkPushState(ctx);
  gkUseProgram(ctx, prog);
  gkDisableDepthTest(ctx);

  if (gk__bone_vao == UINT_MAX)
    gkDrawBonesInit();
  else
    glBindVertexArray(gk__bone_vao);

  gkUniformMat4(gkUniformLoc(prog, "MV"), scene->camera->viewProj);
  glBindBuffer(GL_UNIFORM_BUFFER, gk__bone_ubo);

  sceneImpl = (GkSceneImpl *)scene;
  if ((item = sceneImpl->instSkins)) {
    do {
      node     = item->data;
      ctlrInst = node->controller;

      if (ctlrInst->ctlr
          && (skin = (GkSkin *)ctlrInst->ctlr)
          && (modelInst = skin->base.source)) {
        /* TODO: optimize this */
        glBufferSubData(GL_UNIFORM_BUFFER,
                        0,
                        sizeof(mat4) * skin->nJoints,
                        modelInst->jointsToDraw);
        glDrawArrays(GL_LINE_STRIP, 0, skin->nJoints);
      }
    } while ((item = item->next));
  }

  gkPopState(ctx);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

_gk_hide
void
gkDrawBonesRelease() {
  if (gk__bone_vao == UINT_MAX)
    return;

  glDeleteVertexArrays(1, &gk__bone_vao);

  gk__bone_vao = UINT_MAX;
}
