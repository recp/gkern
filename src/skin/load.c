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
  GkPrimInst         *prim;
  GkGpuBuffer        *gbuff;
  GkVertexAttachment *va, *va_last;
  GkGPUAccessor      *gaccJOINTS, *gaccWEIGHTS;
  GkVertexInput      *viJOINTS, *viWEIGHTS;
  size_t              i;
  uint32_t            primCount, maxJointCount, byteStride;

  primCount = skin->nPrims;

  /* currently allow only 4 joint */
  maxJointCount = 4;
  byteStride    = maxJointCount * (sizeof(float) + sizeof(int));

  for (i = 0; i < primCount; i++) {
    prim                    = &modelInst->prims[i];
    gbuff                   = skin->gbuffs[i];
    va                      = calloc(1, sizeof(*va));
    va->semantic            = GK_VERT_ATTACH_SKIN;
    
    viJOINTS                = gkMakeVertexInput("JOINTS", GL_UNSIGNED_INT, 0);
    gaccJOINTS              = calloc(1, sizeof(*gaccJOINTS));
    gaccJOINTS->buffer      = gbuff;
    gaccJOINTS->itemType    = GL_UNSIGNED_INT;
    gaccJOINTS->byteStride  = byteStride;
    gaccJOINTS->itemCount   = maxJointCount;
    gaccJOINTS->gpuTarget   = gbuff->target;
    viJOINTS->accessor      = gaccJOINTS;

    viWEIGHTS               = gkMakeVertexInput("WEIGHTS", GL_FLOAT, 0);
    gaccWEIGHTS             = calloc(1, sizeof(*gaccWEIGHTS));
    gaccWEIGHTS->buffer     = gbuff;
    gaccWEIGHTS->byteOffset = sizeof(int) * maxJointCount;
    gaccWEIGHTS->itemType   = GL_FLOAT;
    gaccWEIGHTS->byteStride = byteStride;
    gaccWEIGHTS->itemCount  = maxJointCount;
    gaccWEIGHTS->gpuTarget  = gbuff->target;
    viWEIGHTS->accessor     = gaccWEIGHTS;

    gkBindPrimitive(prim->prim);
    glBindBuffer(gbuff->target, gbuff->vbo);
    
    gk_attachInputTo(prim, va, viJOINTS);
    gk_attachInputTo(prim, va, viWEIGHTS);
    
    prim->hasSkin = true;

    if ((va_last = prim->vertexAttachments)) {
      while (va_last->next)
        va_last = va_last->next;
      
      va_last->next = va;
    } else {
      prim->vertexAttachments = va;
    }
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
