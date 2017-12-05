/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "vertex_input.h"
#include "../../include/gk/program.h"
#include "../../include/gk/vertex.h"

#include <ds/rb.h>

#include <stdlib.h>
#include <string.h>

static
int
gkVertexInputCmp(void *key1, void *key2);

RBTree *gk__vertexinp = NULL;

static
int
gkVertexInputCmp(void *key1, void *key2) {
  GkVertexInput *a, *b;
  int cmpres;

  a = key1;
  b = key2;

  if ((cmpres = a->type - b->type) != 0)
    return cmpres;

  if ((cmpres = a->len - b->len) != 0)
    return cmpres;

  if ((cmpres = strcmp(a->name, b->name)) != 0)
    return cmpres;

  return 0;
}

GK_EXPORT
GkVertexInput*
gkMakeVertexInput(const char *name, GkType type, int32_t len) {
  GkVertexInput *vi, *found;

  vi = calloc(1, sizeof(*vi));

  vi->type = type;
  vi->len  = len;
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
gkAddVertexInput(GkProgram *prog, GkVertexInput *input) {
  flist_sp_append(&prog->vertex, input);
  input->refc++;

  return flist_sp_indexof(&prog->vertex, input);
}

GK_EXPORT
void
gkRemoveVertexInput(GkProgram *prog, GkVertexInput *input) {
  if (!flist_sp_contains(&prog->vertex, input))
    return;

  input->refc--;
  flist_sp_remove_by(&prog->vertex, input);
}

GK_EXPORT
int32_t
gkIndexOfVertexInput(GkProgram *prog, GkVertexInput *input) {
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
