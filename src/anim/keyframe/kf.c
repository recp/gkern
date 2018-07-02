/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../common.h"
#include "../../../include/gk/animation.h"
#include "../../../include/gk/animation-keyframe.h"
#include "kf.h"
#include <tm/tm.h>

void
gkInterpolate(GkInterpType type,
              float               t,
              GkValue            *from,
              GkValue            *to,
              GkValue            *dest) {
  switch (type) {
    case GK_INTERP_LINEAR:
      gkValueLerp(from, to, t, dest);
      break;
    case GK_INTERP_STEP:
      break;
    case GK_INTERP_BEZIER:
      break;
    case GK_INTERP_CARDINAL:
      break;
    case GK_INTERP_HERMITE:
      break;
    case GK_INTERP_BSPLINE:
      break;
    default: break;
  }
}

_gk_hide
bool
gkBuiltinKFAnim(GkAnimation *anim,
                GkValue     *to,
                GkValue     *delta) {

  return false;
}

GK_EXPORT
GkKeyFrameAnimation*
gkKeyFrameAnimation(void) {
  GkKeyFrameAnimation *kfa;

  kfa = calloc(1, sizeof(*kfa));
  kfa->base.fnAnimator = gkBuiltinKFAnim;
  kfa->base.delta      = calloc(1, sizeof(*kfa->base.delta));
  kfa->base.nRepeat    = 1;
  kfa->base.isKeyFrame = true;

  return kfa;
}
