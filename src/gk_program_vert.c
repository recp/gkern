/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk/program.h"
#include "../include/gk/vertex.h"

#include <ds/rb.h>

#include <stdlib.h>
#include <string.h>

extern
void
gk_vertFreeAttrib(RBTree *tree, RBNode *node);

static
void
gk_progInitVertInf(GkProgInfo * __restrict prog) {
  prog->attribc             = 0;
  prog->attribl             = 0;
  prog->attribs             = rb_newtree_str();
  prog->attribs->onFreeNode = gk_vertFreeAttrib;
}

GLint
gk_progAttribIndex(GkProgInfo * __restrict prog,
                   const char * __restrict name) {
  GkVertexAttrib *attrib;

  /* use global */
  if (!prog->attribs)
    return gk_vertAttribIndex(name);

  attrib = rb_find(prog->attribs, (void *)name);
  if (attrib != NULL)
    return attrib->index;

  return -1;
}

void
gk_progSetAttribs(GkProgInfo * __restrict prog,
                  size_t      count,
                  const char *names[]) {
  size_t i;

  if (!prog->attribs)
    gk_progInitVertInf(prog);

  /* remove prev attribs (not free) */
  if (prog->attribs->count != 0)
    rb_empty(prog->attribs);

  for (i = 0; i < count; i++)
    (void)gk_progAddAttrib(prog, names[i]);
}

GLint
gk_progAddAttrib(GkProgInfo * __restrict prog,
                 const char * __restrict name) {
  GkVertexAttrib *attrib;

  attrib = rb_find(prog->attribs, (void *)name);
  if (attrib != NULL)
    return attrib->index;

  attrib        = calloc(sizeof(*attrib), 1);
  attrib->name  = strdup(name);
  attrib->index = prog->attribl++;

  rb_insert(prog->attribs,
            (void *)attrib->name,
            attrib);

  return attrib->index;
}
