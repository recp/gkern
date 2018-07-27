/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
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
