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
gkInterpolateChannel(GkChannel * __restrict ch,
                     float                  t,
                     bool                   isReverse,
                     GkValue   * __restrict dest) {
  switch (ch->lastInterp) {
    case GK_INTERP_LINEAR:
      gkValueLerp(&ch->outerv[isReverse],
                  &ch->outerv[!isReverse],
                  t,
                  dest);
      break;
    case GK_INTERP_STEP:
      gkValueLerp(&ch->outerv[isReverse],
                  &ch->outerv[!isReverse],
                  t,
                  dest);
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
                GkChannel   *channel,
                GkValue     *to,
                GkValue     *delta) {
  switch (channel->targetType) {
    case GKT_FLOAT: {
      float *target;

      target  = channel->target;
      *target = delta->s32.floatValue;

      break;
    }
    default:
      break;
  }

  if (channel->isTransform) {
    GkNode      *node;
    GkTransform *tr;

    node = channel->node;

    if (channel->isLocalTransform && (tr = node->trans))
      tr->flags &= ~GK_TRANSF_LOCAL_ISVALID;

    gkApplyTransform(anim->scene, node);
  }

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
