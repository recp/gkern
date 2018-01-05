/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "common.h"
#include "../include/gk/scene.h"
#include "../include/gk/node.h"
#include "../include/gk/opt.h"

#include <ds/hash.h>
#include <string.h>

uint32_t gk_nodesPerPage = 64;

static
void
gkPrepareNode(GkScene * __restrict scene,
              GkNode  * __restrict parentNode,
              GkNode  * __restrict node);

GkNode*
gkAllocNode(struct GkScene * __restrict scene) {
  GkNodePage  *np;
  GkSceneImpl *sceneImpl;
  GkNode      *node;
  size_t       i;

  sceneImpl = (GkSceneImpl *)scene;
  np        = sceneImpl->lastPage;

  if (!np || np->isfull)
    goto nw;

  for (i = 0; i < gk_nodesPerPage; i++) {
    node = &np->nodes[i];
    if (node->flags & GK_NODEF_NODE)
      continue;

    node->flags |= GK_NODEF_NODE;
    return node;
  }

nw:
  np = calloc(1, sizeof(*np) + gk_nodesPerPage * sizeof(GkNode));
  np->isfull = false;
  np->next = sceneImpl->lastPage;

  sceneImpl->lastPage = np;
  if (!sceneImpl->nodePages)
    sceneImpl->nodePages = np;

  node = &np->nodes[0];
  node->flags |= GK_NODEF_NODE;

  return node;
}

GK_EXPORT
void
gkFreeNode(GkScene * __restrict scene,
           GkNode  * __restrict node) {
  node->flags &= ~GK_NODEF_NODE;

  /* todo: free node and all child nodes */
}

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

  if (!(tr = node->trans))
    tr = node->trans = parentNode->trans;

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
    GkLight *light;

    light = node->light;

    gkCalcViewTransf(scene, scene->camera, tr);
    light->flags |= GK_LIGHTF_TRANSFORMED;

    glm_vec_rotate_m4(tr->world,
                      (float *)gk_opt(GK_OPT_LIGHT_DIR),
                      light->dir);

    glm_vec_normalize(light->dir);
  }
}

GK_EXPORT
void
gkApplyTransform(GkScene * __restrict scene,
                 GkNode  * __restrict node) {
  GkTransform  *tr;
  GkNode       *mostParent;

  if (!(tr = node->trans))
    node->trans = scene->trans;

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

  if (!(node->flags & GK_NODEF_HAVE_TRANSFORM))
    return;

  if (node->model) {
    while (camItem) {
      camImpl = camItem->data;
      gkCalcFinalTransf(scene, &camImpl->pub, tr);
      camItem = camItem->next;
    }
  } else if (node->light) {
    GkLight *light;

    light = node->light;

    gkCalcViewTransf(scene, scene->camera, tr);
    light->flags |= GK_LIGHTF_TRANSFORMED;

    glm_vec_rotate_m4(tr->world,
                      (float *)gk_opt(GK_OPT_LIGHT_DIR),
                      light->dir);

    glm_vec_normalize(light->dir);

    versor lightOri;
    glm_quatv(lightOri, -M_PI, GLM_XUP);
  }
}

GK_EXPORT
void
gkApplyView(struct GkScene * __restrict scene,
            GkNode         * __restrict node) {
  GkNodePage  *np;
  GkSceneImpl *sceneImpl;
  GkModelInst *modelInst;
  GkCamera    *cam;
  size_t       i;

  sceneImpl = (GkSceneImpl *)scene;
  np        = sceneImpl->lastPage;
  cam       = scene->camera;

  /* invalidate */
  while (np) {
    for (i = 0; i < gk_nodesPerPage; i++) {
      node = &np->nodes[i];

      /* unallocated node */
      if (!(node->flags & GK_NODEF_NODE)
          || !(modelInst = node->model))
        continue;

      while (modelInst) {
        modelInst->trans->flags |= GK_TRANSF_CALC_VIEW;
        modelInst = modelInst->next;
      }
    }

    np = np->next;
  }

  np = sceneImpl->lastPage;
  while (np) {
    for (i = 0; i < gk_nodesPerPage; i++) {
      node = &np->nodes[i];

      /* unallocated node */
      if (!(node->flags & GK_NODEF_NODE)
          || !(modelInst = node->model))
        continue;

      while (modelInst) {
        if (modelInst->trans->flags & GK_TRANSF_CALC_VIEW) {
          gkCalcFinalTransf(scene, cam, modelInst->trans);
          modelInst->trans->flags &= ~GK_TRANSF_CALC_VIEW;
        }

        modelInst = modelInst->next;
      }
    }

    np = np->next;
  }
}
