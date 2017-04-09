/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk.h"
#include "../include/gk-vertex.h"
#include "../include/gk-rb.h"

static RBTree  *gk_attribIndTree;
static uint32_t gk_attribIndLast;

static const size_t gk_attribNamesCount = 3;
static const char *gk_attribNames[] = {
  "POSITION",
  "NORMAL",
  "TEXTURE0"
};

void
ak_vertFreeAttrib(RBTree *tree, RBNode *node) {
  GkVertexAttrib *attrib;

  if (node == tree->nullNode)
    return;

  attrib = node->val;

  tree->freeFn((char *)attrib->name);
  tree->freeFn(attrib);
}

void
ak_vertFreeAttribConst(RBTree *tree, RBNode *node) {
  GkVertexAttrib *attrib;

  if (node == tree->nullNode)
    return;

  attrib = node->val;
  tree->freeFn(attrib);
}

GLint
gk_vertAttribIndex(const char *name) {
  GkVertexAttrib *attrib;
  attrib = rb_find(gk_attribIndTree, (void *)name);
  if (attrib != NULL)
    return attrib->index;

  return -1;
}

GLint
gk_vertAddAttrib(const char *name) {
  GkVertexAttrib *attrib;

  attrib = rb_find(gk_attribIndTree, (void *)name);
  if (attrib != NULL)
    return attrib->index;

  attrib        = calloc(sizeof(*attrib), 1);
  attrib->name  = name;
  attrib->index = gk_attribIndLast++;

  rb_insert(gk_attribIndTree,
            (void *)attrib->name,
            attrib);

  return attrib->index;
}

GLint
gk_vertRemoveAttrib(const char *name) {
  GkVertexAttrib *attrib;
  GLint index;

  attrib = rb_find(gk_attribIndTree, (void *)name);
  if (attrib != NULL)
    return -1;

  index = attrib->index;
  rb_remove(gk_attribIndTree, (void *)name);

  return index;
}

void
gk_vertSetAttribs(size_t      count,
                  const char *names[]) {
  size_t i;

  /* remove prev attribs (not free) */
  if (gk_attribIndTree->count != 0)
    rb_empty(gk_attribIndTree);

  for (i = 0; i < count; i++)
    (void)gk_vertAddAttrib(names[i]);
}

void
gk_verta_init() {
  gk_attribIndLast = 0;
  gk_attribIndTree = rb_newtree_str();
  gk_attribIndTree->freeFn   = free;
  gk_attribIndTree->freeNode = ak_vertFreeAttribConst;

  gk_vertSetAttribs(gk_attribNamesCount,
                    gk_attribNames);
}

void
gk_verta_deinit() {
  rb_destroy(gk_attribIndTree);
}
