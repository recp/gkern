/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "gk_def_shader.h"

const char*
gk_def_shader_vert() {
  return
#include "gk_default.vert"
  ;
}

const char*
gk_def_shader_frag() {
  return
#include "gk_default.frag"
  ;
}
