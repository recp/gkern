/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "common.h"
#include "../include/gk/scene.h"
#include "../include/gk/node.h"
#include <ds/hash.h>
#include <string.h>

static
void
gkPrepareNode(GkScene * __restrict scene,
              GkNode  * __restrict parentNode,
              GkNode  * __restrict node);

void
gkMakeNodeTransform(GkScene * __restrict scene,
                    GkNode  * __restrict node) {
  GkTransform *tr;

  if (node->trans)
    return;

  tr           = gkAllocTransform(scene);
  node->trans  = tr;
  node->flags |= GK_NODEF_HAVE_TRANSFORM;
}

static
void
gkPrepareNode(GkScene * __restrict scene,
              GkNode  * __restrict parentNode,
              GkNode  * __restrict node) {
  GkSceneImpl  *sceneImpl;
  GkCameraImpl *camImpl;
  FListItem    *camItem;
  GkTransform  *tr;

  sceneImpl = (GkSceneImpl *)scene;
  camItem   = sceneImpl->transfCacheSlots->first;
  tr        = node->trans;

  if (!GK_FLG(tr->flags, GK_TRANSF_LOCAL_ISVALID))
    gkTransformCombine(tr);

  if (parentNode && (node->flags & GK_NODEF_HAVE_TRANSFORM))
    glm_mul(parentNode->trans->world, tr->local, tr->world);

  if (node->bbox)
    gkTransformAABB(tr, node->bbox);

  if (node->model) {
    GkModelInst *modelInst;

    while (camItem) {
      camImpl = camItem->data;
      gkCalcFinalTransf(scene, &camImpl->pub, tr);
      camItem = camItem->next;
    }

    modelInst = node->model;
    do {
      /* this may be moved to prepare scene */
      modelInst->trans = tr;
      if (modelInst->model->bbox) {
        if (!modelInst->bbox) {
          modelInst->bbox = malloc(sizeof(*modelInst->bbox));
          memcpy(modelInst->bbox,
                 modelInst->model->bbox,
                 sizeof(*modelInst->bbox));
        }

        gkTransformAABB(tr, modelInst->bbox);
      }

      modelInst = modelInst->next;
    } while (modelInst);
  } else if (node->light) {
    gkCalcViewTransf(scene, scene->camera, tr);
  }
}

GK_EXPORT
void
gkApplyTransform(GkScene * __restrict scene,
                 GkNode  * __restrict node) {
  GkTransform  *tr;
  GkNode       *mostParent;

  if (!node
      || !(tr = node->trans)
      || !(node->flags & GK_NODEF_HAVE_TRANSFORM))
    return;

  gkPrepareNode(scene, node->parent, node);

  /* do the same for child nodes */
  if (!node->chld)
    return;

  if (!node->parent)
    glm_mul(scene->trans->world, tr->local, tr->world);

  mostParent = node->parent;
  node       = node->chld;
  while (node) {
    gkPrepareNode(scene, node->parent, node);
    while (node->chld) {
      gkPrepareNode(scene, node, node->chld);
      node = node->chld;
    }

    if (node->next) {
      node = node->next;
      continue;
    }

    node = node->parent;
    while (node) {
      if (node == mostParent)
        goto dn;
      if (node->next) {
        node = node->next;
        break;
      }
      node = node->parent;
    }
  }

dn:; /* done */
}

GK_INLINE
void
gkPrepareView(GkScene * __restrict scene,
              GkNode  * __restrict node) {
  GkSceneImpl  *sceneImpl;
  GkCameraImpl *camImpl;
  FListItem    *camItem;
  GkTransform  *tr;

  sceneImpl = (GkSceneImpl *)scene;
  camItem   = sceneImpl->transfCacheSlots->first;
  tr        = node->trans;

  if (node->model) {
    while (camItem) {
      camImpl = camItem->data;
      gkCalcFinalTransf(scene, &camImpl->pub, tr);
      camItem = camItem->next;
    }
  } else if (node->light) {
    gkCalcViewTransf(scene, scene->camera, tr);
  }
}

GK_EXPORT
void
gkApplyView(struct GkScene * __restrict scene,
            GkNode         * __restrict node) {
  GkNode *mostParent;

  if (!node)
    return;

  gkPrepareView(scene, node);

  /* do the same for child nodes */
  if (!node->chld)
    return;

  mostParent = node->parent;
  node       = node->chld;
  while (node) {
    gkPrepareView(scene, node);
    while (node->chld) {
      gkPrepareView(scene, node);
      node = node->chld;
    }

    if (node->next) {
      node = node->next;
      continue;
    }

    node = node->parent;
    while (node) {
      if (node == mostParent)
        goto dn;
      if (node->next) {
        node = node->next;
        break;
      }
      node = node->parent;
    }
  }

dn:; /* done */
}
