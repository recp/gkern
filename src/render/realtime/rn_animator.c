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
  tm_interval  time, endTime;
  float        t, ease;
  uint32_t     finished, finishReq;
  bool         isReverse;

  if (!(animItem = sceneImpl->anims))
    return;

  time  = sceneImpl->pub.startTime;
  v.val = vd.val = NULL;

  do {
    finished  = 0;
    anim      = animItem->data;
    isReverse = anim->isReverse;

    /* 0 means that animation must start with scene rendering */
    if (anim->beginTime == 0)
      anim->beginTime = time;

    /* remove animation */
    if (anim->beginTime > time
        || anim->nRepeat <= anim->nPlayed) {
      continue;
    }

    if (anim->isKeyFrame) {
      GkKeyFrameAnimation *kfa;
      GkChannel           *ch;
      GkAnimSampler       *sampler;
      float               *inp;
      uint32_t             inpLen;

      kfa = (GkKeyFrameAnimation *)anim;
      ch  = kfa->channel;

      finishReq = kfa->channelCount;

      if (ch) {
        while (ch) {
          uint32_t keyIndex;

          sampler  = ch->sampler;
          inp      = sampler->input->data;
          inpLen   = (uint32_t)sampler->input->count;
          keyIndex = ch->keyIndex;

          if (inpLen < 2)
            goto nxt;

          /* first time to run channel */
          if (!ch->isPrepared) {
            gkPrepChannel(anim, ch);

            if (!isReverse) {
              ch->keyIndex   = 1;
              ch->keyEndTime = ch->beginTimeRef + inp[1];
            } else {
              ch->keyIndex   = inpLen - 2;
              ch->keyEndTime = ch->beginTimeRef + ch->duration - inp[inpLen - 2];
            }

            gkPrepChannelKey(kfa, ch);

            /* move object to first location */
            anim->fnKfAnimator(anim, ch, &ch->ov[0], NULL);
            goto nxt;
          }

          if (ch->beginTimeRef > time)
            goto nxt;

          /* finished */
          if (ch->endTimeRef < time) {
            ch->isPreparedKey = ch->isPrepared = false;
            anim->beginTime   = time;

            if (anim->autoReverse) {
              if (isReverse)
                ch->keyIndex = 0;
              else
                ch->keyIndex = inpLen - 1;
            } else {
              ch->keyIndex = 0;
            }

            finished++;
            goto nxt;
          }

          finished = false;

          /* switch to next input */
          while (ch->keyEndTime < time) {
            if (!isReverse) {
              keyIndex++;
              if (keyIndex >= inpLen)
                goto nxt;
            } else {
              if (keyIndex == 0)
                goto nxt;
              keyIndex--;
            }

            ch->keyStartTime  = ch->keyEndTime;

            if (!isReverse) {
              ch->keyEndTime = ch->beginTimeRef + inp[keyIndex];

            } else {
              ch->keyEndTime = ch->beginTimeRef + ch->duration - inp[keyIndex];
            }

            ch->isPreparedKey = false;
            ch->keyIndex      = keyIndex;
          }

          /* clamp index */
          if (!isReverse)
            keyIndex = GLM_MIN(inpLen - 1, keyIndex);
          else
            keyIndex = GLM_MAX(0, keyIndex);

          if (!ch->isPreparedKey) {
            gkPrepChannelKey(kfa, ch);
          }

          t = glm_percentc(ch->keyStartTime, ch->keyEndTime, time);

          if (sampler->uniInterp == GK_INTERP_UNKNOWN) {
            char *interpi;

            interpi = sampler->interp->data;
            if (!isReverse)
              ch->lastInterp = interpi[GLM_MAX(1, keyIndex) - 1];
            else
              ch->lastInterp = interpi[GLM_MIN(inpLen - 2, keyIndex)];
          } else {
            ch->lastInterp = sampler->uniInterp;
          }

          gkInterpolateChannel(ch, time, t, isReverse, &v);

          if (ch->computeDelta) {
            gkValueSub(&v, &ch->delta, &vd);
            gkValueCopy(&v, &ch->delta);
          }

          anim->fnKfAnimator(anim, ch, &v, &vd);

        nxt:
          ch = ch->next;
        }
      }
    } else {
      finishReq = 1;
      endTime   = anim->beginTime + anim->duration;
      t         = glm_percentc(anim->beginTime, endTime, time);
      ease      = anim->fnTiming ? anim->fnTiming(t) : t;

      if (!isReverse) {
        gkValueLerp(anim->from, anim->to, ease, &v);
      } else {
        gkValueLerp(anim->to, anim->from, ease, &v);
      }

      gkValueSub(&v, anim->delta, &vd);
      gkValueCopy(&v, anim->delta);

      anim->fnAnimator(anim, &v, &vd);

      finished = t == 1.0f;
    }

    if (finished == finishReq) {
      if (!anim->autoReverse) {
        anim->nPlayed++;
      } else {
        if (isReverse)
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
