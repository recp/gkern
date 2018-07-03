/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "rn_animator.h"
#include "../../../include/gk/gk.h"
#include "../../../include/gk/animation.h"
#include "../../../include/gk/animation-keyframe.h"
#include "../../anim/keyframe/kf.h"

#include <tm/tm.h>
#include <limits.h>

void
gkRunAnim(GkSceneImpl *sceneImpl) {
  FListItem   *animItem;
  GkAnimation *anim;
  GkValue      v, vd;
  tm_interval  time, endTime, keyEndTime;
  float        t, ease;

  if (!(animItem = sceneImpl->anims))
    return;

  time  = sceneImpl->pub.startTime;
  v.val = vd.val = NULL;

  do {
    anim = animItem->data;

    /* 0 means that animation must start with scene rendering */
    if (anim->beginTime == 0)
      anim->beginTime = time;

    /* remove animation */
    if (anim->beginTime > time
        || anim->nRepeat <= anim->nPlayed) {
      continue;
    }

    endTime = anim->beginTime + anim->duration;
    t       = glm_percentc(anim->beginTime, endTime, time);
    ease    = anim->fnTiming ? anim->fnTiming(t) : t;

    if (anim->isKeyFrame) {
      GkKeyFrameAnimation *kfa;
      GkChannel           *ch;
      GkAnimSampler       *sampler;
      float               *input;
      uint32_t             kfindex;

      kfa     = (GkKeyFrameAnimation *)anim;
      ch = kfa->channel;

      if (ch) {
        while (ch) {
          char        *interpi;
          GkInterpType interp;

          sampler    = ch->sampler;
          input      = sampler->input->data;
          kfindex    = GLM_MIN((uint32_t)sampler->input->len - 1, kfa->kfindex);
          keyEndTime = anim->beginTime + input[kfa->kfindex];

          interpi    = sampler->interp->data;
          interp     = interpi[kfindex];

          if (kfindex == 0 && !ch->isPrepared)
            gkPrepChannel(ch);

          /* switch to next point */
          if (!ch->isPreparedKey)
            gkPrepChannelKey(ch);

          ch->lastInterp = interp;
          gkInterpolateChannel(ch, ease, anim->isReverse, &v);

          gkValueSub(&v, &ch->delta, &vd);
          gkValueCopy(&v, &ch->delta);

          anim->fnKFAnimator(anim, ch, &v, &vd);

          /* TODO: do this after animation of single keyframe completed */
          /* kfa->kfindex++; */

          ch = ch->next;
        }
      }
    } else {
      if (!anim->isReverse) {
        gkValueLerp(anim->from, anim->to, ease, &v);
      } else {
        gkValueLerp(anim->to, anim->from, ease, &v);
      }

      gkValueSub(&v, anim->delta, &vd);
      gkValueCopy(&v, anim->delta);

      anim->fnAnimator(anim, &v, &vd);
    }

    if (t == 1.0f) {
      if (!anim->autoReverse) {
        anim->nPlayed++;
      } else {
        if (anim->isReverse)
          anim->nPlayed++;

        if (anim->nPlayed < anim->nRepeat)
          anim->beginTime = time;

        anim->isReverse = !anim->isReverse;
      }

      if (anim->nRepeat == UINT_MAX)
        anim->beginTime = time;
    }
  } while ((animItem = animItem->next));
}
