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

#include "../common.h"
#include "vertex_input.h"
#include "../../include/gk/program.h"
#include "../../include/gk/vertex.h"

#include <ds/rb.h>

#include <stdlib.h>
#include <string.h>


RBTree *gk__vertexinp = NULL;

GK_EXPORT
int
gkVertexInputCmp(void *key1, void *key2) {
  GkVertexInput *a, *b;
  int cmpres;

  a = key1;
  b = key2;

  if ((cmpres = strcmp(a->name, b->name)) != 0)
    return cmpres;

  if (a->accessor != b->accessor)
    return 1;

  return 0;
}

GK_EXPORT
GkVertexInput*
gkMakeVertexInput(const char *name, GkType type, int32_t len) {
  GkVertexInput *vi, *found;

  vi = calloc(1, sizeof(*vi));

//  vi->type = type;
//  vi->len  = len;
  vi->name = strdup(name);

  if ((found = rb_find(gk__vertexinp, vi))) {
    free((void *)vi->name);
    free(vi);
    return found;
  }

  rb_insert(gk__vertexinp, vi, vi);

  return vi;
}

GK_EXPORT
int32_t
gkAddVertexInput(GkPipeline *prog, GkVertexInput *input) {
  flist_sp_append(&prog->vertex, input);
  input->refc++;

  return flist_sp_indexof(&prog->vertex, input);
}

GK_EXPORT
void
gkRemoveVertexInput(GkPipeline *prog, GkVertexInput *input) {
  if (!flist_sp_contains(&prog->vertex, input))
    return;

  input->refc--;
  flist_sp_remove_by(&prog->vertex, input);
}

GK_EXPORT
int32_t
gkIndexOfVertexInput(GkPipeline *prog, GkVertexInput *input) {
  return flist_sp_indexof(&prog->vertex, input);
}

void
gk_vertinp_init() {
  gk__vertexinp = rb_newtree(NULL, gkVertexInputCmp, NULL);
}

void
gk_vertinp_deinit() {
  rb_destroy(gk__vertexinp);
}
