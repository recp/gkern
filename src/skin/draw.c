/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "draw.h"
#include "../default/def_prog.h"
#include "../program/uniform_cache.h"
#include <limits.h>

GLuint gk__bone_vao = UINT_MAX;

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
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1, 0);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

GK_EXPORT
void
gkDrawBones(GkScene * __restrict scene,
            GkSkin  * __restrict skin) {
  GkPipeline *prog;
  GLuint      currentProg;

  prog        = gk_prog_drawbone();
  currentProg = gkCurrentProgram();

  glUseProgram(prog->progId);

  if (gk__bone_vao == UINT_MAX)
    gkDrawBonesInit();
  else
    glBindVertexArray(gk__bone_vao);

  gkUniformMat4(gkUniformLoc(prog, "MV"), scene->camera->viewProj);

  glDrawArrays(GL_LINE_STRIP, 0, skin->nJoints);
  glUseProgram(currentProg);
}

_gk_hide
void
gkDrawBonesRelease() {
  if (gk__bone_vao == UINT_MAX)
    return;

  glDeleteVertexArrays(1, &gk__bone_vao);

  gk__bone_vao = UINT_MAX;
}
