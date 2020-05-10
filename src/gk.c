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
#include "../include/gk/context.h"
#include "state/gpu.h"

#include <ds/forward-list.h>
#include <string.h>

GkContext*
gkAllocContext() {
  GkContext *ctx;

  ctx = calloc(1, sizeof(*ctx));
  ctx->mdltree   = rb_newtree_ptr();
  ctx->states    = flist_new(NULL);
  ctx->samplers  = flist_new(NULL);

  gkInitStates(ctx);

  return ctx;
}

void
gkContextFree(GkContext *ctx) {
  rb_destroy(ctx->mdltree);
  free(ctx);
}

GkModelInst *
gkMakeInstance(GkModel *model) {
  GkModelInst *inst, *prevInst;
  GkGpuBuffer *uboJoints, *uboTargetWeights;
  int32_t      primc, i;

  primc    = model->primc;
  inst     = calloc(1, sizeof(*inst) + sizeof(GkPrimInst) * primc);
  prevInst = NULL;

  if (!model->instances)
    model->instances = calloc(1, sizeof(*model->instances));
  else
    prevInst = model->instances->instance;

  model->instances->instance = inst;
  model->instances->instanceCount++;
  inst->model = model;

  if (prevInst)
    inst->next = prevInst;

  inst->primc = primc;
  for (i = 0; i < primc; i++) {
    memcpy(&inst->prims[i].bbox,
           &model->prims[i].bbox,
           sizeof(model->prims[i].bbox));

    inst->prims[i].material  = model->prims[i].material;
    inst->prims[i].prim      = &model->prims[i];
    inst->prims[i].modelInst = inst;
  }

  /* create an UBO for joints to share joints between primitives */
  uboJoints         = calloc(1, sizeof(*uboJoints));
  uboJoints->size   = sizeof(mat4) * 255;  /* TODO: */
  uboJoints->usage  = GL_DYNAMIC_DRAW;
  uboJoints->target = GL_UNIFORM_BUFFER;

  glGenBuffers(1, &uboJoints->vbo);
  glBindBuffer(uboJoints->target, uboJoints->vbo);
  glBufferData(uboJoints->target,
               uboJoints->size,
               NULL,
               uboJoints->usage);
  glBindBufferRange(GL_UNIFORM_BUFFER, 1, uboJoints->vbo, 0, uboJoints->size);

  inst->uboJoints = uboJoints;

  /* create an UBO for target weights to share weights between primitives */
  uboTargetWeights         = calloc(1, sizeof(*uboTargetWeights));
  uboTargetWeights->size   = sizeof(float) * 4 * 16;  /* TODO: */
  uboTargetWeights->usage  = GL_DYNAMIC_DRAW;
  uboTargetWeights->target = GL_UNIFORM_BUFFER;

  glGenBuffers(1, &uboTargetWeights->vbo);
  glBindBuffer(uboTargetWeights->target, uboTargetWeights->vbo);
  glBufferData(uboTargetWeights->target,
               uboTargetWeights->size,
               NULL,
               uboTargetWeights->usage);
  glBindBufferBase(uboTargetWeights->target, 2, uboTargetWeights->vbo);

//  glBindBufferRange(GL_UNIFORM_BUFFER,
//                    2,
//                    uboTargetWeights->vbo,
//                    0,
//                    uboTargetWeights->size);
//  glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboTargetWeights->vbo);
//  glBindBuffer(uboTargetWeights->target, 0);

  inst->uboTargetWeights = uboTargetWeights;
  
  return inst;
}

void
gkReshape(GkScene *scene, GkRect rect) {
  if (!scene->camera)
    return;

  gkResizeCamera(scene->camera, rect.size.w / rect.size.h);

  scene->flags         |= GK_SCENEF_RENDER;
  scene->camera->flags |= GK_UPDT_PROJ;

  gkCameraProjUpdated(scene->camera);

  scene->viewport[0] = rect.origin.x;
  scene->viewport[1] = rect.origin.y;
  scene->viewport[2] = rect.size.w;
  scene->viewport[3] = rect.size.h;
}

GK_EXPORT
void
gkGetDepthForPoint(vec3 point) {
  glReadPixels((GLint)point[0],
               (GLint)point[1],
               1,
               1,
               GL_DEPTH_COMPONENT,
               GL_FLOAT,
               &point[2]);
}

GK_EXPORT
void
gkPrimAddBuffer(GkPrimitive * __restrict prim,
                GkGpuBuffer * __restrict buff) {
  prim->bufc++;

  if (prim->bufs)
    prim->bufs->prev = buff;

  buff->next   = prim->bufs;
  prim->bufs = buff;
}

