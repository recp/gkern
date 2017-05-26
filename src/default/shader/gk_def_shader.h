/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_def_shader_h
#define gk_def_shader_h

#include "../../../include/gk.h"

typedef enum GK_DEF_SHADER {
  GK_DEF_SHADER_DEFAULT = 0,
  GK_DEF_SHADER_CUBE    = 1
} GK_DEF_SHADER;

const char*
gk_def_shader_vert(GK_DEF_SHADER shader);

const char*
gk_def_shader_frag(GK_DEF_SHADER shader);

#endif /* gk_def_shader_h */
