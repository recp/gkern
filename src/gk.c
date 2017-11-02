/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk/gk.h"
#include <ds/forward-list.h>
#include "gpu_state/default.h"

GkContext*
gkContextNew(GkProgInfo * __restrict pinfo) {
  GkContext *ctx;

  ctx = malloc(sizeof(*ctx));
  ctx->mdltree = rb_newtree_ptr();

  if (!pinfo)
    ctx->pinfo = gkDefaultProgram();
  else
    ctx->pinfo = pinfo;
  
  ctx->states = flist_new(NULL);
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

void
gkUniformMatrix(GkModelInst *modelInst) {
  GkModel          *model;
  GkFinalTransform *ftr;
  GkProgInfo       *pinfo;
  int               usenm;

  model = modelInst->model;

  ftr   = modelInst->trans->ftr;
  pinfo = model->pinfo;

  /* Model View Projection Matrix */
  gkUniformMat4(pinfo->mvpi, ftr->mvp);

  /* Model View Matrix */
  gkUniformMat4(pinfo->mvi, ftr->mv);

  /* Normal Matrix */
  usenm = (modelInst->trans->flags & GK_TRANSF_FMAT_NORMAT) != 0;

  if (usenm)
    gkUniformMat4(pinfo->nmi,  ftr->nm);

  glUniform1i(pinfo->nmui, usenm);
}
