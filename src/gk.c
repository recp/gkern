/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "common.h"
#include "../include/gk/gk.h"
#include "gpu_state/default.h"

#include <ds/forward-list.h>

GkContext*
gkContextNew(GkProgram * __restrict prog) {
  GkContext *ctx;

  ctx = calloc(sizeof(*ctx), 1);
  ctx->mdltree = rb_newtree_ptr();

  if (!prog)
    ctx->prog = gkDefaultProgram();
  else
    ctx->prog = prog;

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

  instance     = calloc(sizeof(*instance), 1);
  prevInstance = NULL;

  if (!model->instances)
    model->instances = calloc(sizeof(*model->instances), 1);
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

  primInst = calloc(sizeof(*primInst), 1);
  primInst->prim     = prim;
  primInst->material = NULL;

  rb_insert(modelInst->prims, prim, primInst);

  return primInst;
}
