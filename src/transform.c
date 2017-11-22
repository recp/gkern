/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "common.h"
#include "../include/gk/gk.h"
#include "../include/gk/scene.h"
#include "../include/gk/geom-types.h"
#include "types/impl_transform.h"

#include <ds/forward-list.h>
#include <string.h>

GK_EXPORT
void
gkCacheTransformsFor(GkScene  * __restrict scene,
                     GkCamera * __restrict cam) {
  GkCameraImpl *camImpl;
  FList        *transfCacheSlots;
  
  camImpl             = (GkCameraImpl *)cam;
  transfCacheSlots    = scene->_priv.transfCacheSlots;

  flist_append(transfCacheSlots, cam);
  
  camImpl->transfSlot = flist_indexof(transfCacheSlots, cam);
}

GK_EXPORT
void
gkRemoveTransformCacheFor(GkScene  * __restrict scene,
                          GkCamera * __restrict cam) {
  GkCameraImpl *camImpl;
  FList        *transfCacheSlots;

  camImpl             = (GkCameraImpl *)cam;
  transfCacheSlots    = scene->_priv.transfCacheSlots;
  camImpl->transfSlot = (1 << 31);

  flist_remove_by(transfCacheSlots, cam);
}

GK_EXPORT
GkTransform*
gkAllocTransform(GkScene * __restrict scene) {
  GkTransformImpl *trans;
  uint32_t         slotCount;

  slotCount = (uint32_t)scene->_priv.transfCacheSlots->count;
  trans     = calloc(sizeof(*trans), 1);
  if (slotCount > 0) {
    trans->ftr  = calloc(sizeof(GkFinalTransform *) * slotCount, 1);
    trans->ftrc = slotCount;
  }

  return &trans->pub;
}

GK_EXPORT
void
gkResizeTransform(GkScene         * __restrict scene,
                  GkTransformImpl * __restrict trans) {
  uint32_t slotCount;

  slotCount = (uint32_t)scene->_priv.transfCacheSlots->count;
  if (trans->ftrc == slotCount)
    return;

  if (slotCount == 0 && trans->ftrc != 0) {
    free(trans->ftr);
    return;
  }

  if (!trans->ftr)
    trans->ftr = calloc(sizeof(void *) * slotCount, 1);
  else
    trans->ftr = realloc(trans->ftr, sizeof(void *) * slotCount);

  trans->ftrc = slotCount;
}

void
gkTransformCombine(GkTransform * __restrict trans) {
  mat4 mat = GLM_MAT4_IDENTITY_INIT;
  GkTransformItem *ti;

  if ((ti = trans->item)) {
    mat4 tmp;

    do {
      switch (ti->type) {
        case GK_TRANS_MATRIX: {
          GkMatrix *matrix;
          matrix = (GkMatrix *)ti;

          glm_mat4_mul(mat, matrix->value, mat);
          break;
        }
        case GK_TRANS_LOOK_AT: {
          GkLookAt *lookAt;
          lookAt = (GkLookAt *)ti;

          glm_lookat(lookAt->value[0],
                     lookAt->value[1],
                     lookAt->value[2],
                     tmp);

          /* because this is view matrix */
          glm_inv_tr(tmp);
          glm_mat4_mul(mat, tmp, mat);
          break;
        }
        case GK_TRANS_ROTATE: {
          GkRotate *rotate;

          rotate = (GkRotate *)ti;
          glm_rotate_make(tmp, rotate->value[3], rotate->value);
          glm_mat4_mul(mat, tmp, mat);
          break;
        }
        case GK_TRANS_SCALE: {
          GkScale *scale;
          scale = (GkScale *)ti;

          glm_scale_make(tmp, scale->value);
          glm_mat4_mul(mat, tmp, mat);
          break;
        }
        case GK_TRANS_TRANSLATE: {
          GkTranslate *translate;
          translate = (GkTranslate *)ti;

          glm_translate_make(tmp, translate->value);
          glm_mat4_mul(mat, tmp, mat);
          break;
        }
        case GK_TRANS_SKEW: {
          /* TODO:
             GkSkew *skew;
             skew = (GkSkew *)ti;

             glm_mat4_mul(mat, tmp, mat);
           */
          break;
        }
        /* unitialized transform? */
        default:
          goto ret;
          break;
      }

      ti = ti->next;
    } while (ti);
  }

ret:
  glm_mat4_copy(mat, trans->local);

  trans->flags |= GK_TRANSF_LOCAL_ISVALID;
}

GkPoint
gk_project2d(GkRect rect, mat4 mvp, vec3 v) {
  vec4    pos4;
  GkPoint p;

  glm_vec_copy(v, pos4);
  pos4[3] = 1.0f;

  glm_mat4_mulv(mvp, pos4, pos4);
  glm_vec4_scale(pos4, 1.0f / pos4[3], pos4); /* pos = pos / pos.w */

  p.x = rect.origin.x + rect.size.w * (pos4[0] + 1.0f) * 0.5f;
  p.y = rect.origin.y + rect.size.h * (pos4[1] + 1.0f) * 0.5f;
  return p;
}

void
gkUniformTransform(struct GkProgram * __restrict prog,
                   GkTransform      * __restrict trans,
                   GkCamera         * __restrict cam) {
  GkFinalTransform *ftr;
  vec4             *pmvp, *pmv, *pnm;
  mat4              mvp, mv, nm;
  int               usenm;
  int32_t           hasMVP, hasMV, hasNM;

  hasMVP = prog->mvpi > -1;
  hasMV  = prog->mvi  > -1;
  hasNM  = prog->nmi  > -1;

  /* no need to uniform transform or invalid program configurations */
  if (!(hasMVP | hasMV | hasNM))
    return;

  pmvp = pmv = pnm = NULL;
  if ((ftr = gkFinalTransform(trans, cam))) {
    pmvp = ftr->mvp;
    pmv  = ftr->mv;
    pnm  = ftr->nm;
  } else {
    pmvp = mvp;
    pmv  = mv;
    pnm  = nm;
  }

  /* Model View Projection Matrix */
  if (hasMVP) {
    if (!ftr)
      glm_mul(cam->projView, trans->world, mvp);

    gkUniformMat4(prog->mvpi, pmvp);
  }

  /* Model View Matrix */
  if (hasMV) {
    if (!ftr)
      glm_mul(cam->view, trans->world, mv);

    gkUniformMat4(prog->mvi, pmv);
  }

  /* Normal Matrix */
  if (hasNM) {
    usenm = !(trans->flags & GK_TRANSF_FMAT_NORMAT);
    if (usenm) {
      if (!ftr) {
        glm_mat4_inv(pmv, nm);
        glm_mat4_transpose(nm);
      }

      gkUniformMat4(prog->nmi, pnm);
    }

    glUniform1i(prog->nmui, usenm);
  }
}

void
gkCalcFinalTransf(GkScene     * __restrict scene,
                  GkCamera    * __restrict cam,
                  GkTransform * __restrict tr) {
  GkFinalTransform *ftr;
  GkCameraImpl     *camImpl;

  camImpl = (GkCameraImpl *)cam;
  if (camImpl->transfSlot == (1 << 31))
    return;

  if (!(ftr = gkFinalTransform(tr, cam)))
    ftr = gkSetFinalTransform(scene, tr, cam);

  glm_mat4_mul(cam->view, tr->world, ftr->mv);
  glm_mat4_mul(cam->proj, ftr->mv,   ftr->mvp);

  if (glm_uniscaled(tr->world)) {
    tr->flags &= ~GK_TRANSF_FMAT_NORMAT;
  } else {
    tr->flags |= GK_TRANSF_FMAT_NORMAT;

    glm_mat4_inv(ftr->mv, ftr->nm);
    glm_mat4_transpose(ftr->nm);
  }

  tr->flags |= (GK_TRANSF_FMAT | GK_TRANSF_FMAT_MV | GK_TRANSF_FMAT_MVP);
}

void
gkCalcViewTransf(GkScene     * __restrict scene,
                 GkCamera    * __restrict cam,
                 GkTransform * __restrict tr) {
  GkFinalTransform *ftr;
  GkCameraImpl     *camImpl;

  camImpl = (GkCameraImpl *)cam;
  if (camImpl->transfSlot == (1 << 31))
    return;

  if (!(ftr = gkFinalTransform(tr, cam)))
    ftr = gkSetFinalTransform(scene, tr, cam);

  glm_mat4_mul(cam->view, tr->world, ftr->mv);

  tr->flags |= (GK_TRANSF_FMAT | GK_TRANSF_FMAT_MV);
}
