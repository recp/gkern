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
void
gkPrepChannel(GkChannel *ch) {
  if (!ch->isPrepared) {
    GkBuffer *output;
    output = ch->sampler->output;

    switch (ch->targetType) {
      case GKT_FLOAT: {
        gkInitValueAsFloat(&ch->outerv[0], *(float *)ch->target);
        if (output->len > 0)
          gkInitValueAsFloat(&ch->outerv[1],
                             *(float *)output->data - 1);
        break;
      }
      case GKT_FLOAT3: {
        gkInitValueAsVec3(&ch->outerv[0], ch->target);

        if (output->len > 2)
          gkInitValueAsVec3(&ch->outerv[1],
                            output->data + output->len - 3);
        break;
      }
      default: break;
    }
  }

  ch->keyv[0].type
    = ch->keyv[0].type
    = ch->outerv[1].type
    = ch->outerv[0].type;
}

GK_EXPORT
void
gkPrepChannelKey(GkChannel *ch) {
  GkBuffer *output;

  output = ch->sampler->output;

  if (!ch->isPreparedKey) {
    switch (ch->targetType) {
      case GKT_FLOAT: {
        float *target;

        target = output->data;
        gkInitValueAsFloat(&ch->keyv[0], *target);

        break;
      }
      case GKT_FLOAT3: {
        float *target;

        target = output->data;
        gkInitValueAsVec3(&ch->keyv[0], target);

        break;
      }
      default: break;
    }
  }

  ch->keyv[0].type
  = ch->keyv[0].type
  = ch->outerv[1].type
  = ch->outerv[0].type;
}

GK_EXPORT
GkKeyFrameAnimation*
gkKeyFrameAnimation(void) {
  GkKeyFrameAnimation *kfa;

  kfa = calloc(1, sizeof(*kfa));
  kfa->base.fnKFAnimator = gkBuiltinKFAnim;
  kfa->base.delta        = calloc(1, sizeof(*kfa->base.delta));
  kfa->base.nRepeat      = 1;
  kfa->base.isKeyFrame   = true;

  return kfa;
}
