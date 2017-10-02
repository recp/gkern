/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "shader.h"

#include "../../include/gk/shader.h"
#include "../../include/gk/material.h"
#include <ds/rb.h>

static RBTree *gk_shaders;

GkShader*
gkShaderByName(const char *name) {
  return rb_find(gk_shaders, (void *)name);
}

void
gkShaderSetName(GkShader *shader, const char *name) {
  void *found;
  if ((found = rb_find(gk_shaders, (void *)name)))
    rb_remove(gk_shaders, (void *)name);

  rb_insert(gk_shaders, (void *)name, shader);
}

void
gk_shaders_init() {
  gk_shaders = rb_newtree_str();
}

void
gk_shaders_deinit() {
  rb_destroy(gk_shaders);
}
