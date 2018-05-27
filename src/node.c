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
#include "bbox/scene_bbox.h"
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

  if (!np || np->size == np->count)
    goto nw;

  for (i = 0; i < gk_nodesPerPage; i++) {
    node = &np->nodes[i];
    if (node->flags & GK_NODEF_NODE)
      continue;

    node->flags |= GK_NODEF_NODE;
    np->count++;
    return node;
  }

nw:
  np        = calloc(1, sizeof(*np) + gk_nodesPerPage * sizeof(GkNode));
  np->size  = gk_nodesPerPage;
  np->next  = sceneImpl->lastPage;
  np->count = 1;

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
  GkLight      *light;
  int32_t       modelCount;
  bool          finalComputed;

  sceneImpl = (GkSceneImpl *)scene;
  camItem   = sceneImpl->transfCacheSlots->first;

  finalComputed = false;
  if (!(tr = node->trans))
    tr = node->trans = parentNode->trans;

  if (!GK_FLG(tr->flags, GK_TRANSF_LOCAL_ISVALID))
    gkTransformCombine(tr);

  if (parentNode && (node->flags & GK_NODEF_HAVE_TRANSFORM))
    glm_mul(parentNode->trans->world, tr->local, tr->world);

  /* TODO: */
  /* gkTransformAABB(tr, node->bbox); */

  modelCount = 0;

  if (node->model) {
    GkModelInst *modelInst;

    glm_vec_scale(scene->center,
                  sceneImpl->centercount,
                  scene->center);

    while (camItem) {
      camImpl = camItem->data;
      gkCalcFinalTransf(scene, &camImpl->pub, tr);
      camItem = camItem->next;
    }

    finalComputed = true;
    modelInst     = node->model;

    do {
      GkPrimInst *prims;
      vec3        modelCenter;
      int32_t     i, primc;

      modelInst->trans = tr;
      glm_aabb_transform(modelInst->model->bbox,
                         tr->world,
                         modelInst->bbox);

      prims = modelInst->prims;
      primc = modelInst->primc;

      glm_vec_zero(modelCenter);

      for (i = 0; i < primc; i++) {
        glm_aabb_transform(prims[i].prim->bbox,
                           tr->world,
                           prims[i].bbox);

        gkUpdateSceneAABB(scene, prims[i].bbox);
        prims[i].trans = tr;
      }

      glm_mat4_mulv3(tr->world,
                     modelInst->model->center,
                     1.0f,
                     modelInst->center);

      if (modelInst->addedToScene) {
        glm_vec_sub(scene->center, modelInst->center, scene->center);
      } else {
        modelInst->addedToScene = true;
        sceneImpl->centercount++;
      }

      glm_vec_add(scene->center, modelInst->center, scene->center);

      modelInst = modelInst->next;
    } while (modelInst);

    glm_vec_divs(scene->center,
                 sceneImpl->centercount,
                 scene->center);
  }

  if ((light = node->light)) {
    if (!finalComputed) {
      while (camItem) {
        camImpl = camItem->data;
        gkCalcViewTransf(scene, &camImpl->pub, tr);
        camItem = camItem->next;
      }
    }

    light->flags |= GK_LIGHTF_TRANSFORMED;
    glm_vec_rotate_m4(tr->world, light->defdir, light->dir);
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
  GkLight      *light;
  bool          finalComputed;

  sceneImpl = (GkSceneImpl *)scene;
  camItem   = sceneImpl->transfCacheSlots->first;
  tr        = node->trans;

  if (!(tr->flags & GK_TRANSF_CALC_VIEW))
    return;

  finalComputed = false;
  if (node->model) {
    while (camItem) {
      camImpl = camItem->data;
      gkCalcFinalTransf(scene, &camImpl->pub, tr);
      camItem = camItem->next;
    }

    finalComputed = true;
  }

  if ((light = node->light)) {
    if (!finalComputed) {
      while (camItem) {
        camImpl = camItem->data;
        gkCalcViewTransf(scene, &camImpl->pub, tr);
        camItem = camItem->next;
      }
    }

    light->flags |= GK_LIGHTF_TRANSFORMED;
    glm_vec_rotate_m4(tr->world, light->defdir, light->dir);
    glm_vec_normalize(light->dir);
  }
}

GK_EXPORT
void
gkApplyView(struct GkScene * __restrict scene,
            GkNode         * __restrict node) {
  GkNodePage  *np;
  GkSceneImpl *sceneImpl;
  GkModelInst *modelInst;
  size_t       i;

  sceneImpl = (GkSceneImpl *)scene;
  np        = sceneImpl->lastPage;

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
      if (!(node->flags & GK_NODEF_NODE))
        continue;

      if (node->model || node->light)
        gkPrepareView(scene, node);
    }

    np = np->next;
  }
}
