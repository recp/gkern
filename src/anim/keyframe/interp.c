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
gkInterpolateChannel(GkAnimation * __restrict anim,
                     GkChannel   * __restrict ch,
                     double                   time,
                     float                    t,
                     bool                     isReverse) {
  GkValue *curr;
  GkValue *p0Val;
  float   *otnv, *itnv;
  uint32_t index, itemc;

  curr  = &ch->curr;
  p0Val = &ch->kv[isReverse];
  index = ch->keyIndex;

  itemc = p0Val->rowCount;
  if (!ch->currValuePrepared) {
    gkInitValue(curr, GKT_FLOAT, itemc, 1, sizeof(float));
    ch->currValuePrepared = true;
  }

  switch (ch->lastInterp) {
    case GK_INTERP_LINEAR:
      if (ch->property == GK_TARGET_QUAT) {
        vec4 rot;
        glm_quat_slerp(ch->kv[isReverse].val, ch->kv[!isReverse].val, t, rot);
        gkInitValueAsVec4(curr, rot);
      } else {
        gkValueLerp(&ch->kv[isReverse], &ch->kv[!isReverse], t, curr);
      }
      break;
    case GK_INTERP_STEP:
      gkValueCopy(&ch->kv[t < 1.0f && isReverse], curr);
      break;
    case GK_INTERP_BEZIER:
    case GK_INTERP_HERMITE:
    case GK_INTERP_BSPLINE:
    case GK_INTERP_CARDINAL: {
      GkBuffer *otn, *itn;
      float    *p0, *p1, *c0, *c1, *Bs, T0, s, keyBeginAt, keyEndAt;
      uint32_t  i, j;

      itn        = ch->sampler->inTangent;
      otn        = ch->sampler->outTangent;
      otnv       = otn->data;
      itnv       = itn->data;
      p0         = p0Val->val;
      p1         = ch->kv[!isReverse].val;
      c0         = otnv + (index - 1) * ch->sampler->outTangentStride;
      c1         = itnv + (index - 1) * ch->sampler->inTangentStride;
      T0         = time - anim->beginTime;
      keyBeginAt = ch->keyBeginTime - anim->beginTime;
      keyEndAt   = ch->keyEndTime   - anim->beginTime;
      Bs         = curr->val;

      switch (ch->lastInterp) {
        case GK_INTERP_BEZIER: {
          for (i = 0; i < itemc; i++) {
            j     = i * 2;
            s     = glm_decasteljau(T0, keyBeginAt, c0[j], c1[j], keyEndAt);
            Bs[i] = glm_bezier(s, p0[i], c0[j + 1], c1[j + 1], p1[i]);
          }
          break;
        }
        case GK_INTERP_HERMITE: {
          for (i = 0; i < itemc; i++) {
            j     = i * 2;
            s     = glm_decasteljau(T0, keyBeginAt, c0[j], c1[j], keyEndAt);
            Bs[i] = glm_hermite(s, p0[i], c0[j + 1], c1[j + 1], p1[i]);
          }
        }
        default: break;
      }

      break;
    }
    default: break;
  }
}
