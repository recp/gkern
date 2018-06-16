/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "rn_anim.h"
#include "../../../include/gk/gk.h"
#include "../../../include/gk/animation.h"

#include <tm/tm.h>
#include <limits.h>

void
gkRunAnim(GkSceneImpl *sceneImpl) {
  FListItem   *animItem;
  GkAnimation *anim;
  GkValue      v, vd;
  tm_interval  time, endTime;
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
        || anim->repeatCount <= anim->playedCount) {
      continue;
    }

    endTime = anim->beginTime + anim->duration;
    t       = glm_percentc(anim->beginTime, endTime, time);
    ease    = anim->timingFunc ? anim->timingFunc(t) : t;

    if (!anim->isReverse) {
      gkValueLerp(anim->from, anim->to, ease, &v);
    } else {
      gkValueLerp(anim->to, anim->from, ease, &v);
    }

    gkValueSub(&v, anim->delta, &vd);
    gkValueCopy(&v, anim->delta);

    anim->cb(anim, &v, &vd);

    if (t == 1.0f) {
      if (!anim->autoReverse) {
        anim->playedCount++;
      } else {
        if (anim->isReverse)
          anim->playedCount++;

        if (anim->playedCount < anim->repeatCount)
          anim->beginTime = time;

        anim->isReverse = !anim->isReverse;
      }

      if (anim->repeatCount == UINT_MAX)
        anim->beginTime = time;
    }
  } while ((animItem = animItem->next));
}
