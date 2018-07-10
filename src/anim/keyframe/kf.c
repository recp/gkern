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
    case GK_INTERP_BEZIER: {
      /*
       BEZIER:
         B(s) = P0*(1 - s)^3 + 3*C0*s*(1 - s)^2 + 3*C1*s^2*(1 - s)+ P1*s^3
       */
      GkBuffer *otn, *itn;
      uint32_t  keyIndex;

      itn      = ch->sampler->inTangent;
      otn      = ch->sampler->outTangent;
      keyIndex = ch->keyIndex - 1;

      switch (ch->targetType) {
        case GKT_FLOAT: {
          float Bs, s, ss, tt, p0, p1, c0, c1, *otnv, *itnv;

          otnv = otn->data;
          itnv = itn->data;
          p0   = ch->kv[isReverse].s32.floatValue;
          p1   = ch->kv[!isReverse].s32.floatValue;
          s    = 1.0f - t;
          ss   = s * s;
          tt   = t * t;
          c0   = otnv[keyIndex];
          c1   = itnv[keyIndex];

          Bs = p0 * ss * s
                  + 3.0f * c0 * t * ss
                  + 3.0f * c1 * tt * s
                  + p1 * tt * t;

          gkInitValueAsFloat(dest, Bs);
          break;
        }
        case GKT_FLOAT3: {
          vec3  Bs, tmp0, tmp1, tmp2;
          float s, ss, tt, *p0, *p1, *c0, *c1, *otnv, *itnv;

          otnv = otn->data;
          itnv = itn->data;
          p0   = ch->kv[isReverse].val;
          p1   = ch->kv[!isReverse].val;
          s    = 1.0f - t;
          ss   = s * s;
          tt   = t * t;
          c0   = otnv + keyIndex * 3;
          c1   = itnv + keyIndex * 3;

          glm_vec_scale(p0, ss * s, Bs);
          glm_vec_scale(c0, 3.0f * t * ss, tmp0);
          glm_vec_scale(c1, 3.0f * tt * s, tmp1);
          glm_vec_scale(p1, tt * t, tmp2);

          glm_vec_add(Bs, tmp0, Bs);
          glm_vec_add(Bs, tmp1, Bs);
          glm_vec_add(Bs, tmp2, Bs);

          gkInitValueAsVec3(dest, Bs);
          break;
        }
        default:
          break;
      }
      break;
    }
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
      glm_vec_copy(to->val, ch->target);
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
