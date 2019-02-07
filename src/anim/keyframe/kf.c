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

_gk_hide
bool
gkBuiltinKeyAnim(GkAnimation *anim,
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
    case GKT_FLOAT3: {
      glm_vec3_copy(to->val, ch->target);
      break;
    }
    case GKT_FLOAT4: {
      glm_vec4_ucopy(to->val, ch->target);
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
    double    beginTime;
    int       isReverse;

    outp             = ch->sampler->output;
    data             = outp->data;
    oLen             = outp->len;
    isReverse        = anim->isReverse;
    beginTime        = anim->beginTime + ch->beginAt;

    ch->beginTime    = beginTime;
    ch->keyBeginTime = beginTime;
    ch->keyEndTime   = beginTime;
    ch->endTime      = anim->beginTime + ch->endAt;

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
      case GKT_FLOAT4: {
        gkInitValueAsVec4(&ch->ov[isReverse], ch->target);

        if (outp->len > 2)
          gkInitValueAsVec4(&ch->ov[!isReverse],
                            (float *)(data + oLen - sizeof(vec4)));
        break;
      }
      default: break;
    }
  }

  ch->kv[0].type
    = ch->kv[1].type
    = ch->ov[0].type
    = ch->ov[1].type;

  /* fix 1D tangents */
  if ((int)ch->lastInterp >= (int)GK_INTERP_BEZIER)
    gkGenTangentKeysIfNeeded(ch);

  ch->isPrepared = true;
}

GK_EXPORT
void
gkPrepChannelKey(GkKeyFrameAnimation *anim, GkChannel *ch) {
  GkBuffer *output, *input;
  uint32_t  index, prevIndex;
  int       isReverse;

  output    = ch->sampler->output;
  input     = ch->sampler->input;
  index     = ch->keyIndex;
  isReverse = anim->base.isReverse;

  if (!anim->base.isReverse)
    prevIndex = GLM_MAX(1, index) - 1;
  else
    prevIndex = GLM_MIN((uint32_t)input->count - 2, index) + 1;

  switch (ch->targetType) {
    case GKT_FLOAT: {
      float *target;

      target = output->data;

      gkInitValueAsFloat(&ch->kv[isReverse],  target[prevIndex]);
      gkInitValueAsFloat(&ch->kv[!isReverse], target[index]);

      break;
    }
    case GKT_FLOAT3: {
      float *target;

      target = output->data;

      gkInitValueAsVec3(&ch->kv[isReverse],  target + 3 * prevIndex);
      gkInitValueAsVec3(&ch->kv[!isReverse], target + 3 * index);

      break;
    }
    case GKT_FLOAT4: {
      float *target;

      target = output->data;

      gkInitValueAsVec4(&ch->kv[isReverse],  target + 4 * prevIndex);
      gkInitValueAsVec4(&ch->kv[!isReverse], target + 4 * index);
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
  kfa->base.fnKfAnimator = gkBuiltinKeyAnim;
  kfa->base.delta        = calloc(1, sizeof(*kfa->base.delta));
  kfa->base.nRepeat      = 1;
  kfa->base.isKeyFrame   = true;
  kfa->base.autoReverse  = false;

  return kfa;
}
