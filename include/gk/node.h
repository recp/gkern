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
#include "camera.h"
#include "transform.h"

#include <stdlib.h>
#include <cglm/cglm.h>

struct GkScene;
struct GkAnimatable;

typedef enum GkNodeFlags {
  GK_NODEF_NODE           = 1,
  GK_NODEF_HAVE_TRANSFORM = 2
} GkNodeFlags;

typedef struct GkNode {
  struct GkAnimatable  *anim;
  GkNodeFlags           flags;

  GkTransform          *trans;

  GkModelInst          *model;
  GkLight              *light;
  GkCamera             *camera;

  struct GkNode        *parent;
  struct GkNode        *next;
  struct GkNode        *chld;
} GkNode;

GkNode*
gkAllocNode(struct GkScene * __restrict scene);

void
gkMakeNodeTransform(struct GkScene * __restrict scene,
                    GkNode         * __restrict node);

GK_EXPORT
void
gkApplyTransform(struct GkScene * __restrict scene,
                 GkNode         * __restrict node);

GK_EXPORT
void
gkApplyView(struct GkScene * __restrict scene,
            GkNode         * __restrict node);

#ifdef __cplusplus
}
#endif
#endif /* gk_node_h */
