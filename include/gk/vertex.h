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

#ifndef gk_vertex_h
#define gk_vertex_h

#include "common.h"
#include "program.h"
#include "type.h"

typedef struct GkVertexInput {
  const char           *name;
  GkType                type;
  int32_t               len;
  int32_t               refc;
} GkVertexInput;

GK_EXPORT
GkVertexInput*
gkMakeVertexInput(const char *name, GkType type, int32_t len);

GK_EXPORT
int32_t
gkAddVertexInput(GkPipeline *prog, GkVertexInput *input);

GK_EXPORT
void
gkRemoveVertexInput(GkPipeline *prog, GkVertexInput *input);

GK_EXPORT
int32_t
gkIndexOfVertexInput(GkPipeline *prog, GkVertexInput *input);

GK_EXPORT
const char*
gkShortNameOfVertexInput(const char *name);

GK_EXPORT
const char*
gkSetShortNameOfVertexInput(const char *name, const char *shortName);

#endif /* gk_vertex_h */
