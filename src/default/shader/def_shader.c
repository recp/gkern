/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "def_shader.h"

const char*
gk_def_shader_vert(GK_DEF_SHADER shader) {

  switch (shader) {
    case GK_DEF_SHADER_CUBE:
      return
#include "gk_cube.vert"
      ;
      break;
    default:
      return
#include "gk_default.vert"
      ;
  }
}

const char*
gk_def_shader_frag(GK_DEF_SHADER shader) {
  switch (shader) {
    case GK_DEF_SHADER_CUBE:
      return
#include "gk_cube.frag"
      ;
      break;
    default:
      return
#include "gk_default.frag"
      ;
  }
}
