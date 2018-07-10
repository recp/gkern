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

GK_EXPORT
void
gkInterpolateChannel(GkChannel * __restrict ch,
                     float                  t,
                     bool                   isReverse,
                     GkValue   * __restrict dest) {
  switch (ch->lastInterp) {
    case GK_INTERP_LINEAR:
      gkValueLerp(&ch->kv[isReverse], &ch->kv[!isReverse], t, dest);
      break;
    case GK_INTERP_STEP:
      gkValueCopy(&ch->kv[t < 1.0f && isReverse], dest);
      break;
    case GK_INTERP_BEZIER:
      break;
    case GK_INTERP_HERMITE:
      break;
    case GK_INTERP_CARDINAL:
      break;
    case GK_INTERP_BSPLINE:
      break;
    default: break;
  }
}

_gk_hide
bool
gkBuiltinKeyFrmAnim(GkAnimation *anim,
                    GkChannel   *ch,
                    GkValue     *to,
                    GkValue     *delta) {
  switch (ch->targetType) {
    case GKT_FLOAT: {
      float *target;

      target  = ch->target;
      *target = to->s32.floatValue;

      break;
    }
    default: break;
  }

  if (ch->isTransform) {
    GkNode      *node;
    GkTransform *tr;

    node = ch->node;

    if (ch->isLocalTransform && (tr = node->trans))
      tr->flags &= ~GK_TRANSF_LOCAL_ISVALID;

    gkApplyTransform(anim->scene, node);
  }

  return false;
}

GK_EXPORT
void
gkPrepChannel(GkAnimation *anim, GkChannel *ch) {
  if (!ch->isPrepared) {
    GkBuffer *outp;
    char     *data;
    size_t    oLen;
    int       isReverse;

    outp      = ch->sampler->output;
    data      = outp->data;
    oLen      = outp->len;
    isReverse = anim->isReverse;

    ch->beginTimeRef = ch->beginTime + anim->beginTime;
    ch->endTimeRef   = ch->endTime   + anim->beginTime;
    ch->keyStartTime = ch->keyEndTime = ch->beginTimeRef;

    switch (ch->targetType) {
      case GKT_FLOAT: {
        gkInitValueAsFloat(&ch->ov[isReverse], *(float *)ch->target);

        if (outp->len > 0)
          gkInitValueAsFloat(&ch->ov[!isReverse],
                             *(float *)(data + oLen - sizeof(float)));
        break;
      }
      case GKT_FLOAT3: {
        gkInitValueAsVec3(&ch->ov[isReverse], ch->target);

        if (outp->len > 2)
          gkInitValueAsVec3(&ch->ov[!isReverse],
                            (float *)(data + oLen - sizeof(vec3)));
        break;
      }
      default: break;
    }
  }

  ch->kv[0].type
    = ch->kv[1].type
    = ch->ov[0].type
    = ch->ov[1].type;

  ch->isPrepared = true;
}

GK_EXPORT
void
gkPrepChannelKey(GkKeyFrameAnimation *anim, GkChannel *ch) {
  GkBuffer *output, *input;
  uint32_t  keyIndex, prevKeyIndex;
  int       isReverse;

  output    = ch->sampler->output;
  input     = ch->sampler->input;
  keyIndex  = ch->keyIndex;
  isReverse = anim->base.isReverse;

  if (!anim->base.isReverse)
    prevKeyIndex = GLM_MAX(1, keyIndex) - 1;
  else
    prevKeyIndex = GLM_MIN((uint32_t)input->count - 2, keyIndex) + 1;

  switch (ch->targetType) {
    case GKT_FLOAT: {
      float *target;

      target = output->data;

      gkInitValueAsFloat(&ch->kv[isReverse],  target[prevKeyIndex]);
      gkInitValueAsFloat(&ch->kv[!isReverse], target[keyIndex]);

      break;
    }
    case GKT_FLOAT3: {
      float *target;

      target = output->data;

      gkInitValueAsVec3(&ch->kv[isReverse],  target + 3 * prevKeyIndex);
      gkInitValueAsVec3(&ch->kv[!isReverse], target + 3 * keyIndex);

      break;
    }
    default: break;
  }

  ch->isPreparedKey = true;
}

GK_EXPORT
GkKeyFrameAnimation*
gkKeyFrameAnimation(void) {
  GkKeyFrameAnimation *kfa;

  kfa                    = calloc(1, sizeof(*kfa));
  kfa->base.fnKfAnimator = gkBuiltinKeyFrmAnim;
  kfa->base.delta        = calloc(1, sizeof(*kfa->base.delta));
  kfa->base.nRepeat      = 1;
  kfa->base.isKeyFrame   = true;
  kfa->base.autoReverse  = false;

  return kfa;
}
