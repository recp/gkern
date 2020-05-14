/*
 * This file is part of the gk project (https://github.com/recp/gk)
 * Copyright (c) Recep Aslantas.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "../../common.h"
#include "../../../include/gk/animation.h"
#include "../../../include/gk/animation-keyframe.h"
#include "kf.h"
#include <tm/tm.h>

GK_EXPORT
void
gkChannelSetTargetTo(GkChannel        * __restrict ch,
                     GkFloatOrPointer * __restrict to) {
  float   *target, *tov;
  uint32_t stride, i;

  stride = ch->stride;
  target = ch->target;

  if (stride == 1) {
    *target = to->s32;
  } else {
    tov = to->p;
    for (i = 0; i < stride; i++)
      target[i] = tov[i];
  }
}

_gk_hide
bool
gkBuiltinKeyAnim(GkAnimation *anim,
                 GkChannel   *ch,
                 GkValue     *to,
                 GkValue     *delta) {
  float   *target, *tov;
  uint32_t stride, i;
    
  stride = ch->stride;
  target = ch->target;

  if (stride == 1) {
    *target = to->s32.floatValue;
  } else {
    tov = to->val;
    for (i = 0; i < stride; i++)
      target[i] = tov[i];
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
    float    *target, *data;
    double    beginTime;
    int       isReverse, stride;

    outp             = ch->sampler->output;
    data             = outp->data;
    isReverse        = anim->isReverse;
    beginTime        = anim->beginTime + ch->beginAt;
    stride           = ch->stride;
    target           = ch->target;

    ch->beginTime    = beginTime;
    ch->keyBeginTime = beginTime;
    ch->keyEndTime   = beginTime;
    ch->endTime      = anim->beginTime + ch->endAt;

    ch->ov[isReverse]  = target;
    ch->ov[!isReverse] = data + (outp->count - 1) * stride;
  }

  /* fix 1D tangents */
  if ((int)ch->lastInterp >= (int)GK_INTERP_BEZIER)
    gkGenTangentKeysIfNeeded(ch);

  ch->isPrepared = true;
}

GK_EXPORT
void
gkPrepChannelKey(GkKeyFrameAnimation *anim, GkChannel *ch) {
  GkBuffer *output, *input;
  float    *data;
  uint32_t  index, prevIndex;
  int       isReverse, stride;

  output    = ch->sampler->output;
  input     = ch->sampler->input;
  index     = ch->keyIndex;
  isReverse = anim->base.isReverse;
  stride    = ch->stride;
  data      = output->data;

  if (!anim->base.isReverse)
    prevIndex = GLM_MAX(1, index) - 1;
  else
    prevIndex = GLM_MIN((uint32_t)input->count - 2, index) + 1;

  ch->kv[isReverse]  = data + stride * prevIndex;
  ch->kv[!isReverse] = data + stride * index;
  
  ch->isPreparedKey = true;
}

GK_EXPORT
GkKeyFrameAnimation*
gkKeyFrameAnimation(void) {
  GkKeyFrameAnimation *kfa;

  kfa                    = calloc(1, sizeof(*kfa));
  /* kfa->base.fnKfAnimator = gkBuiltinKeyAnim; */
  kfa->base.delta        = calloc(1, sizeof(*kfa->base.delta));
  kfa->base.nRepeat      = 1;
  kfa->base.isKeyFrame   = true;
  kfa->base.autoReverse  = false;

  return kfa;
}
