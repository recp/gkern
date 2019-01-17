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

#define DECASTEL_EPS   1e-9
#define DECASTEL_MAX   32
#define DECASTEL_SMALL 1e-20

GK_EXPORT
float
gkDeCasteljau(float t, float p0, float c0, float c1, float p1) {
  double u, v, a, b, c, d, e, f;
  int    i;

  /*
   References:
   [0] https://forums.khronos.org/showthread.php/10264-Animations-in-1-4-1-release-notes-revision-A/page2?highlight=bezier
   [1] https://forums.khronos.org/showthread.php/10644-Animation-Bezier-interpolation
   [2] https://forums.khronos.org/showthread.php/10387-2D-Tangents-in-Bezier-Splines?p=34164&viewfull=1#post34164
   */

  if (t - p0 < DECASTEL_SMALL)
    return 0.0;

  if (p1 - t < DECASTEL_SMALL)
    return 1.0;

  u  = 0.0f;
  v  = 1.0f;

  for (i = 0; i < DECASTEL_MAX; i++) {
    /* de Casteljau Subdivision */
    a  = (p0 + c0) * 0.5f;
    b  = (c0 + c1) * 0.5f;
    c  = (c1 + p1) * 0.5f;
    d  = (a  + b)  * 0.5f;
    e  = (b  + c)  * 0.5f;
    f  = (d  + e)  * 0.5f; /* this one is on the curve! */

    /* The curve point is close enough to our wanted t */
    if (fabs(f - t) < DECASTEL_EPS)
      return glm_clamp_zo((u  + v) * 0.5f);

    /* dichotomy */
    if (f < t) {
      p0 = f;
      c0 = e;
      c1 = c;
      u  = (u  + v)  * 0.5f;
    } else {
      c0 = a;
      c1 = d;
      p1 = f;
      v  = (u  + v)  * 0.5f;
    }
  }

  return glm_clamp_zo((u  + v)  * 0.5f);
}

GK_EXPORT
void
gkInterpolateChannel(GkChannel * __restrict ch,
                     double                 time,
                     float                  t,
                     bool                   isReverse,
                     GkValue   * __restrict dest) {
  float *otnv, *itnv;

  switch (ch->lastInterp) {
    case GK_INTERP_LINEAR:
      if (ch->property == GK_TARGET_QUAT) {
        vec4 rot;
        glm_quat_slerp(ch->kv[isReverse].val,
                       ch->kv[!isReverse].val,
                       t,
                       rot);
        gkInitValueAsVec4(dest, rot);
      } else {
        gkValueLerp(&ch->kv[isReverse], &ch->kv[!isReverse], t, dest);
      }
      break;
    case GK_INTERP_STEP:
      gkValueCopy(&ch->kv[t < 1.0f && isReverse], dest);
      break;
    case GK_INTERP_BEZIER: {
      GkBuffer *otn, *itn;
      uint32_t  keyIndex;

      itn      = ch->sampler->inTangent;
      otn      = ch->sampler->outTangent;
      keyIndex = ch->keyIndex - 1;

      switch (ch->targetType) {
        case GKT_FLOAT: {
          float Bs, p0, p1, c0, c1;

          otnv = otn->data;
          itnv = itn->data;
          p0   = ch->kv[isReverse].s32.floatValue;
          p1   = ch->kv[!isReverse].s32.floatValue;
          c0   = otnv[keyIndex * 2 + 1];
          c1   = itnv[keyIndex * 2 + 1];

          /* TODO use gkDeCasteljau ? */
          Bs = glm_bezier_cubic(t, p0, c0, c1, p1);
          gkInitValueAsFloat(dest, Bs);
          break;
        }
        case GKT_FLOAT3: {
          vec3  Bs;
          float *p0, *p1, *c0, *c1;

          otnv = otn->data;
          itnv = itn->data;
          p0   = ch->kv[isReverse].val;
          p1   = ch->kv[!isReverse].val;
          c0   = otnv + keyIndex * 3 + 1;
          c1   = itnv + keyIndex * 3 + 1;

          glm_bezier_cubicv3(t, p0, c0, c1, p1, Bs);
          gkInitValueAsVec3(dest, Bs);
          break;
        }
        default:
          break;
      }
      break;
    }
    case GK_INTERP_HERMITE: {
      GkBuffer *otn, *itn;
      uint32_t  keyIndex;

      itn      = ch->sampler->inTangent;
      otn      = ch->sampler->outTangent;
      keyIndex = ch->keyIndex - 1;

      switch (ch->targetType) {
        case GKT_FLOAT: {
          float Hs, p0, p1, t0, t1;

          otnv = otn->data;
          itnv = itn->data;
          p0   = ch->kv[isReverse].s32.floatValue;
          p1   = ch->kv[!isReverse].s32.floatValue;
          t0   = otnv[keyIndex * 2 + 1];
          t1   = itnv[keyIndex * 2 + 1];

          Hs = glm_hermite_cubic(t, p0, t0, t1, p1);
          gkInitValueAsFloat(dest, Hs);
          break;
        }
        case GKT_FLOAT3: {
          vec3  Hs;
          float *p0, *p1, *t0, *t1;

          otnv = otn->data;
          itnv = itn->data;
          p0   = ch->kv[isReverse].val;
          p1   = ch->kv[!isReverse].val;
          t0   = otnv + keyIndex * 3 + 1;
          t1   = itnv + keyIndex * 3 + 1;

          glm_hermite_cubicv3(t, p0, t0, t1, p1, Hs);
          gkInitValueAsVec3(dest, Hs);
          break;
        }
        default:
          break;
      }
      break;
    }
    case GK_INTERP_BSPLINE:
      break;
    case GK_INTERP_CARDINAL:
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
