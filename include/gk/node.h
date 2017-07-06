/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_node_h
#define gk_node_h
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include "light.h"
#include "bbox.h"
#include "model.h"
#include "transform.h"
  
#include <stdlib.h>
#include <cglm.h>

typedef struct GkNode {
  GkNodeFlags    flags;
  GkProgInfo    *pinfo;    /* program info, node may use different shaders */
  GkTransform   *trans;    /* transform */
  GkModelInst   *model;    /* model instance, only instances! */
  GkLight       *light;    /* TODO: save space if there is no light */
  GkBBox        *bbox;
  struct GkNode *next;
  struct GkNode *chld;
  struct GkNode *nodeInst; /* node instance */
} GkNode;

void
gkMakeNodeTransform(GkNode * __restrict node);

#ifdef __cplusplus
}
#endif
#endif /* gk_node_h */