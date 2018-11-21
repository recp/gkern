/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
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
  GkGpuBuffer *uboJoints;
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

#ifdef DEBUG
    assert(!inst->prims[i].material);
#endif
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
  glBindBuffer(uboJoints->target, 0);

  inst->uboJoints = uboJoints;

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

