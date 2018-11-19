/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef builtin_shader_h
#define builtin_shader_h

#include "../../include/gk/gk.h"
#include "../../include/gk/shader.h"

typedef enum GkBuiltinProg {
  GK_BUILTIN_PROG_RTT           = 1,
  GK_BUILTIN_PROG_DEPTH         = 2,
  GK_BUILTIN_PROG_SHADOWMAP     = 3,

  /* weighted-blended transparency */
  GK_BUILTIN_PROG_WEIGBL_COMPOS = 4,

  /* Clear Effects */
  GK_BUILTIN_PROG_CLR_GRAD_CIRC = 5
} GkBuiltinProg;

GkPipeline*
gkBuiltinProg(GkBuiltinProg progtype);

#endif /* builtin_shader_h */
