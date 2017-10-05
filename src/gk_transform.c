/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk/gk.h"
#include "../include/gk/geom-types.h"
#include "../include/gk/transform.h"

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
gkUniformTransform(struct GkProgInfo * __restrict pinfo,
                   GkTransform       * __restrict trans) {
  GkFinalTransform *ftr;
  int               usenm;

  ftr = trans->ftr;

  /* Model View Projection Matrix */
  gkUniformMat4(pinfo->mvpi, ftr->mvp);

  /* Model View Matrix */
  gkUniformMat4(pinfo->mvi, ftr->mv);

  /* Normal Matrix */
  usenm = (trans->flags & GK_TRANSF_FMAT_NORMAT) != 0;

  if (usenm)
  gkUniformMat4(pinfo->nmi,  ftr->nm);

  glUniform1i(pinfo->nmui, usenm);
}

