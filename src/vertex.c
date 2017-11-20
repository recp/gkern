/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "common.h"
#include "vertex.h"

#include "../include/gk/gk.h"
#include "../include/gk/vertex.h"
#include <ds/rb.h>

#include <string.h>

static RBTree  *gk__vertAttrShortNames;

static const size_t gk__vertAttribNamesCount = 13;
static const char *gk__attribNames[] = {
  "POSITION",
  "NORMAL",
  "TEXCOORD",
  "TEXCOORD0",  "TEXCOORD1", "TEXCOORD2", "TEXCOORD3", "TEXCOORD4",
  "TEXCOORD5",  "TEXCOORD6", "TEXCOORD7", "TEXCOORD8", "TEXCOORD9"
};

static const char *gk_attribShortNames[] = {
  "P", "N",
  "T0", "T0", "T1", "T2", "T3", "T4", "T5", "T6", "T7", "T8", "T9"
};

GK_EXPORT
const char*
gkShortNameOfVertexInput(const char *name) {
  return rb_find(gk__vertAttrShortNames, (void *)name);
}

GK_EXPORT
const char*
gkSetShortNameOfVertexInput(const char *name, const char *shortName) {
  return rb_find(gk__vertAttrShortNames,
                 strdup(shortName));
}

void
gk_verta_init() {
  int32_t i;

  gk__vertAttrShortNames = rb_newtree_str();

  for (i = 0; i < gk__vertAttribNamesCount; i++) {
    rb_insert(gk__vertAttrShortNames,
              (void *)gk__attribNames[i],
              (void *)gk_attribShortNames[i]);
  }
}

void
gk_verta_deinit() {
  rb_destroy(gk__vertAttrShortNames);
}
