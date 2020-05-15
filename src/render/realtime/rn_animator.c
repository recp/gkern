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
  GkValue      vd;
  tm_interval  time, endTime;
  float        t, ease;
  uint32_t     finished, finishReq;
  bool         isReverse;

  if (!(animItem = sceneImpl->anims))
    return;

  time     = sceneImpl->pub.startTime;
  vd.val.p = NULL;

  do {
    anim      = animItem->data;
    isReverse = anim->isReverse;

    /* 0 means that animation must start with scene rendering */
    if (anim->beginTime == 0)
      anim->beginTime = time;

    /* remove animation */
    if (anim->beginTime > time || anim->nRepeat <= anim->nPlayed)
      continue;

    if (anim->isKeyFrame) {
      GkKeyFrameAnimation *kfa;
      GkChannel           *ch;
      GkAnimSampler       *sampler;
      float               *inp;
      uint32_t             inpLen;

      kfa      = (GkKeyFrameAnimation *)anim;
      ch       = kfa->channel;
      finished = 0;

      finishReq = kfa->channelCount;

      if (ch) {
        do {
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
              ch->keyEndTime = anim->beginTime + inp[1];
            } else {
              ch->keyIndex   = inpLen - 2;
              ch->keyEndTime = anim->beginTime + ch->duration - inp[inpLen - 2];
            }

            /* move object to first location */
            gkChannelSetTargetTo(ch, &((GkFloatOrPointer){.p = ch->ov[0]}));
            goto nxt;
          }

          /* it is not started yet. */
          if (ch->beginTime > time)
            goto nxt;

#ifdef DEBUG
          assert(ch->endTime >= ch->keyEndTime);
#endif
          /* channel key is finished (all keys), reverse animation if needed */
          if (ch->endTime < time) {
            ch->isFinished = true;
            finished++;

            /* make sure that animation is finished */
            if (ch->keyPercent < 1.0f) {
              gkChannelSetTargetTo(ch, &((GkFloatOrPointer){.p = ch->ov[0]}));
              ch->keyPercent = 1.0f;
            }

            goto nxt;
          }

          /* key is finished, switch to next key */
          while (ch->keyEndTime < time) {
            if (!isReverse) {
              keyIndex++;
              /* channel is finished */
              if (keyIndex >= inpLen) {
                ch->isPreparedKey = ch->isPrepared = false;
                goto nxt;
              }
            } else {
              /* channel is finished */
              if (keyIndex == 0) {
                ch->isPreparedKey = ch->isPrepared = false;
                goto nxt;
              }
              keyIndex--;
            }

            /* shift key */
            ch->keyBeginTime = ch->keyEndTime;

            if (!isReverse) {
              ch->keyEndTime = anim->beginTime + inp[keyIndex];
            } else {
              ch->keyEndTime = anim->beginTime + ch->duration - inp[keyIndex];
            }

            /* invalidate key */
            ch->isPreparedKey = false;
          } /* while */

          /* clamp key index */
          if (!isReverse)
            keyIndex = GLM_MIN(inpLen - 1, keyIndex);
          else
            keyIndex = GLM_MAX(0, keyIndex);

          ch->keyIndex = keyIndex;

          if (!ch->isPreparedKey)
            gkPrepChannelKey(kfa, ch);

          ch->keyPercent = t = glm_percentc(ch->keyBeginTime,
                                            ch->keyEndTime,
                                            time);

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

          gkInterpolateChannel(anim, ch, time, t, isReverse);

          /*
          if (ch->computeDelta) {
            gkValueSub(&ch->curr, &ch->delta, &vd);
            gkValueCopy(&ch->curr, &ch->delta);
          }

          anim->fnKfAnimator(anim, ch, &ch->curr, &vd);
          */
        nxt:
          while ((ch = ch->next) && ch->isFinished) {
            finished++;
          }
        } while (ch);

        /* invalidate all channels */
        if (finished == finishReq) {
          ch = kfa->channel;
          while (ch) {
            ch->isFinished    = false;
            ch->isPreparedKey = false;
            ch->isPrepared    = false;
            ch = ch->next;
          }
        }
      }
    } else {
      finishReq = 1;
      endTime   = anim->beginTime + anim->duration;
      t         = glm_percentc(anim->beginTime, endTime, time);
      ease      = anim->fnTiming ? anim->fnTiming(t) : t;

      if (!isReverse) {
        gkValueLerp(anim->from, anim->to, ease, &anim->curr);
      } else {
        gkValueLerp(anim->to, anim->from, ease, &anim->curr);
      }

      gkValueSub(&anim->curr, anim->delta, &vd);
      gkValueCopy(&anim->curr, anim->delta);

      anim->fnAnimator(anim, &anim->curr, &vd);

      finished = t == 1.0f;
    }

    if (finished == finishReq) {
      if (!anim->autoReverse) {
        anim->nPlayed++;
      } else {
        if (isReverse)
          anim->nPlayed++;

        anim->isReverse = !anim->isReverse;
      }

      if (anim->nPlayed < anim->nRepeat
          || anim->nRepeat == UINT_MAX)
        anim->beginTime = time;
    }

  } while ((animItem = animItem->next));
}
