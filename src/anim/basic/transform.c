/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../common.h"
#include "../../../include/gk/animation.h"
#include <tm/tm.h>

static
_gk_hide
bool
gkBuiltinRotationAnim(GkAnimation *anim,
                      GkValue     *to,
                      GkValue     *delta) {
  GkNode *node;
  vec3   *data;

  node = anim->node;
  data = anim->data;

  glm_rotate_at(node->trans->local,
                data[0],
                delta->s32.floatValue,
                data[1]);

  gkApplyTransform(anim->scene, node);

  return false;
}

static
_gk_hide
bool
gkBuiltinRotationAnimCenter(GkAnimation *anim,
                            GkValue     *to,
                            GkValue     *delta) {
  return gkBuiltinRotationAnim(anim, to, delta);
}

static
_gk_hide
bool
gkBuiltinTranslateAnim(GkAnimation *anim,
                       GkValue     *to,
                       GkValue     *delta) {
  GkNode *node;

  node = anim->node;
  glm_translate(node->trans->local, delta->val);
  gkApplyTransform(anim->scene, node);

  return false;
}

static
_gk_hide
bool
gkBuiltinScaleAnim(GkAnimation *anim,
                   GkValue     *to,
                   GkValue     *delta) {
  GkNode *node;

  node = anim->node;
  glm_scale_make(node->trans->local, to->val);
  gkApplyTransform(anim->scene, node);

  return false;
}

GK_EXPORT
GkAnimation*
gkRotateAnimation(float  to,
                  vec3   pivot,
                  vec3   axis,
                  double duration) {
  GkAnimation *anim;
  vec3        *data;

  anim = calloc(1, sizeof(*anim));
  data = malloc(sizeof(vec3) * 2);

  glm_vec3_copy(pivot, data[0]);
  glm_vec3_copy(axis,  data[1]);

  anim->duration = duration;
  anim->to       = gkValueFromFloat(to);
  anim->delta    = calloc(1, sizeof(*anim->delta));
  anim->data     = data;
  anim->from     = gkValueFromFloat(0.0f);
  anim->nRepeat  = 1;

  if (glm_vec3_eqv(pivot, GK_PIVOT_CENTER)) {
    anim->fnAnimator = gkBuiltinRotationAnimCenter;
  } else {
    anim->fnAnimator = gkBuiltinRotationAnim;
  }

  return anim;
}

GK_EXPORT
GkAnimation*
gkScaleAnimation(vec3   to,
                 double duration) {
  GkAnimation *anim;

  anim = calloc(1, sizeof(*anim));

  anim->duration   = duration;
  anim->to         = gkValueFromVec3(to);
  anim->delta      = calloc(1, sizeof(*anim->delta));
  anim->fnAnimator = gkBuiltinScaleAnim;
  anim->from       = gkValueFromVec3(GLM_VEC3_ONE);
  anim->nRepeat    = 1;

  return anim;
}

GK_EXPORT
GkAnimation*
gkTranslateAnimation(vec3   to,
                     double duration) {
  GkAnimation *anim;

  anim = calloc(1, sizeof(*anim));

  anim->duration    = duration;
  anim->to          = gkValueFromVec3(to);
  anim->delta       = calloc(1, sizeof(*anim->delta));
  anim->fnAnimator  = gkBuiltinTranslateAnim;
  anim->from        = gkValueFromVec3(GLM_VEC3_ZERO);
  anim->nRepeat     = 1;

  return anim;
}
