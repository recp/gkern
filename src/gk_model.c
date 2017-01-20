/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "gk_model.h"
#include "../include/gk-rb.h"

static RBTree *gk_mdltree;

void
gk_mdl_init() {
  gk_mdltree = rb_newtree_ptr();
}

void
gk_model_add(GkModelBase * __restrict model,
             void        * __restrict source) {
  rb_insert(gk_mdltree, source, model);
}

GkModelBase*
gk_model_find(void * __restrict source) {
  return rb_find(gk_mdltree, source);
}

void
gk_mdl_deinit() {
  rb_destroy(gk_mdltree);
}
