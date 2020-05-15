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
  float   *otnv, *itnv, *p0, *p1, *target;
  uint32_t index, stride, i;

  if (!ch->isPreparedKey)
    gkPrepChannelKey((GkKeyFrameAnimation *)anim, ch);

  p0     = ch->kv[isReverse];
  p1     = ch->kv[!isReverse];
  target = ch->target;
  index  = ch->keyIndex;
  stride = ch->stride;

  switch (ch->lastInterp) {
    case GK_INTERP_LINEAR:
      if (ch->property == GK_TARGET_QUAT) {
        glm_quat_slerp(p0, p1, t, target);
      } else {
        for (i = 0; i < stride; i++)
          target[i] = glm_lerp(p0[i], p1[i], t);
      }
      break;
    case GK_INTERP_STEP:
      for (i = 0; i < stride; i++)
        target[i] = *ch->kv[t < 1.0f && isReverse];
      break;
    case GK_INTERP_BEZIER:
    case GK_INTERP_HERMITE:
    case GK_INTERP_BSPLINE:
    case GK_INTERP_CARDINAL: {
      GkBuffer *otn, *itn;
      float    *c0, *c1, *Bs, T0, s, keyBeginAt, keyEndAt;
      uint32_t  j;

      itn        = ch->sampler->inTangent;
      otn        = ch->sampler->outTangent;
      otnv       = otn->data;
      itnv       = itn->data;
      c0         = otnv + (index - 1) * ch->sampler->outTangentStride;
      c1         = itnv + (index - 1) * ch->sampler->inTangentStride;
      T0         = time - anim->beginTime;
      keyBeginAt = ch->keyBeginTime - anim->beginTime;
      keyEndAt   = ch->keyEndTime   - anim->beginTime;
      Bs         = target;

      switch (ch->lastInterp) {
        case GK_INTERP_BEZIER: {
          for (i = 0; i < stride; i++) {
            j     = i * 2;
            s     = glm_decasteljau(T0, keyBeginAt, c0[j], c1[j], keyEndAt);
            Bs[i] = glm_bezier(s, p0[i], c0[j + 1], c1[j + 1], p1[i]);
          }
          break;
        }
        case GK_INTERP_HERMITE: {
          for (i = 0; i < stride; i++) {
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

  if (ch->computeDelta)
    gkVectorSubf(p0, p1, ch->delta.val.p, ch->stride);
  
  if (ch->isTransform && ch->node) {
    if (ch->isLocalTransform && ch->node->trans)
      ch->node->trans->flags &= ~GK_TRANSF_LOCAL_ISVALID;
  
    gkApplyTransform(anim->scene, ch->node);
  }
}
