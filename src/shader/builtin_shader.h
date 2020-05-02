/*
 * This file is part of the gk project (https://github.com/recp/gk)
 * Copyright (c) Recep Aslantas.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
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
