/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../include/gk/gk.h"
#include "../../include/gk/clear.h"
#include "../../include/gk/pass.h"
#include "../../include/gk/gpu_state.h"
#include "../../include/gk/prims/builtin-prim.h"
#include "../../include/gk/vertex.h"
#include "../types/impl_scene.h"
#include "../program/uniform_cache.h"
#include "../shader/builtin_shader.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

GK_EXPORT
void
gkAttachSkin(GkSkin * __restrict skin) {
  GkModelInst *modelInst;
  modelInst = skin->base.source;

  gkAttachSkinTo(skin, modelInst);
}

GK_EXPORT
void
gkAttachSkinTo(GkSkin * __restrict skin, GkModelInst * __restrict modelInst) {
  GkPrimInst    *prim;
  GkGPUBuffer   *gbuff;
  size_t         i;
  uint32_t       primCount, maxJointCount, byteStride;

  primCount = skin->nPrims;

  /* currently allow only 4 joint */
  maxJointCount = 4;
  byteStride    = maxJointCount * (sizeof(float) + sizeof(int));

  for (i = 0; i < primCount; i++) {
    prim = &modelInst->prims[i];

    gkBindPrimitive(prim->prim);

    gbuff = skin->gbuffs[i];

    glBindBuffer(gbuff->target, gbuff->vbo);
    glVertexAttribIPointer(prim->prim->lastInputIndex,
                          maxJointCount,
                          GL_INT,
                          byteStride,
                          BUFFER_OFFSET(0));

    glEnableVertexAttribArray(prim->prim->lastInputIndex);
    prim->prim->lastInputIndex++;

    glVertexAttribPointer(prim->prim->lastInputIndex,
                          maxJointCount,
                          GL_FLOAT,
                          GL_FALSE,
                          byteStride,
                          BUFFER_OFFSET(sizeof(int) * maxJointCount));
    glEnableVertexAttribArray(prim->prim->lastInputIndex);
    prim->prim->lastInputIndex++;

    flist_sp_append(&prim->prim->inputs,
                    gkMakeVertexInput("JOINTS", GL_INT, 0));
    flist_sp_append(&prim->prim->inputs,
                    gkMakeVertexInput("JOINTWEIGHTS", GL_FLOAT, 0));
  }

  modelInst->skin = skin;
}

GK_EXPORT
void
gkUniformJoints(GkScene     * __restrict scene,
                GkModelInst * __restrict modelInst) {
  GkSkin *skin;

  skin = modelInst->skin;

  glBindBuffer(GL_UNIFORM_BUFFER, modelInst->uboJoints->vbo);
  glBufferSubData(GL_UNIFORM_BUFFER,
                  0,
                  sizeof(mat4) * skin->nJoints,
                  modelInst->joints);
  glBindBuffer(modelInst->uboJoints->target, 0);
}

GK_EXPORT
void
gkMakeInstanceSkin(GkScene          * __restrict scene,
                   GkNode           * __restrict node,
                   GkControllerInst * __restrict ctlrInst) {
  GkSceneImpl *sceneImpl;

  sceneImpl        = (GkSceneImpl *)scene;
  ctlrInst->next   = node->controller;
  node->controller = ctlrInst;

  flist_sp_insert(&sceneImpl->instSkins, node);
}
