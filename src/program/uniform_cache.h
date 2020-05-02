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

#ifndef src_uniform_cache_h
#define src_uniform_cache_h

#include "../../include/gk/program.h"
#include "../../include/gk/gk.h"

GK_EXPORT
GLint
gkUniformLoc(GkPipeline *prog, const char *name);

GLint
gkUniformLocBuff(GkPipeline * __restrict prog,
                 char      * __restrict name,
                 char      * __restrict buf);

#endif /* src_uniform_cache_h */

