/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "common.h"
#include "../include/gk/context.h"
#include "gpu_state/default.h"

#include <ds/forward-list.h>

GkContext*
gkAllocContext() {
  GkContext *ctx;

  ctx = calloc(1, sizeof(*ctx));
  ctx->mdltree  = rb_newtree_ptr();
  ctx->states   = flist_new(NULL);
  ctx->samplers = flist_new(NULL);

  gkSetDefaultState(ctx);

  return ctx;
}

void
gkContextFree(GkContext *ctx) {
  rb_destroy(ctx->mdltree);
  free(ctx);
}

GkModelInst *
gkMakeInstance(GkModel *model) {
  GkModelInst *instance, *prevInstance;

  instance     = calloc(1, sizeof(*instance));
  prevInstance = NULL;

  if (!model->instances)
    model->instances = calloc(1, sizeof(*model->instances));
  else
    prevInstance = model->instances->instance;

  model->instances->instance = instance;
  model->instances->instanceCount++;
  instance->model = model;

  if (prevInstance)
    instance->next = prevInstance;

  return instance;
}

GkPrimInst*
gkMakePrimInst(GkModelInst *modelInst,
               GkPrimitive *prim) {
  GkPrimInst *primInst;

  if (!modelInst->prims)
    modelInst->prims = rb_newtree_ptr();

  primInst = rb_find(modelInst->prims, prim);
  if (primInst)
    return primInst;

  primInst = calloc(1, sizeof(*primInst));
  primInst->prim     = prim;
  primInst->material = NULL;

  rb_insert(modelInst->prims, prim, primInst);

  return primInst;
}

void
gkReshape(GkScene *scene, GkRect rect) {
  gkResizeCamera(scene->camera,  rect.size.w / rect.size.h);

  scene->flags         |= GK_SCENEF_RENDER;
  scene->camera->flags |= GK_UPDT_PROJ;

  gkCameraProjUpdated(scene->camera);

  scene->vrect = rect;
}
