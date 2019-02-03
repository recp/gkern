/*iğtikaf*/
/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../common.h"
#include "../../../include/gk/animation.h"
#include "../../../include/gk/animation-keyframe.h"

GK_EXPORT
void
gkGenInTangentKeys(GkChannel * __restrict ch) {
  GkBuffer *itn;
  float    *inp, *nw, *old;
  size_t    count, i, j;

  itn   = ch->sampler->inTangent;
  inp   = ch->sampler->input->data;
  old   = itn->data;
  nw    = malloc(itn->len * 2);
  count = ch->sampler->input->len / sizeof(float);

  for (i = 0; i < count - 1; i++) {
    j = i * 2;
    nw[j]     = inp[i] * 0.3333333333 + inp[i + 1] * 0.6666666667;
    nw[j + 1] = old[i];

    printf("i old[i]: %f %f\n", old[i], glm_deg(old[i]));
  }

  free(old);
  itn->data = nw;
  ch->sampler->inTangentStride = ch->sampler->inTangentStride * 2;
}

GK_EXPORT
void
gkGenOutTangentKeys(GkChannel * __restrict ch) {
  GkBuffer *otn;
  float    *inp, *nw, *old;
  size_t    count, i, j;

  otn   = ch->sampler->outTangent;
inp   = ch->sampler->input->data;
  old   = otn->data;
  nw    = malloc(otn->len * 2);
  count = ch->sampler->input->len / sizeof(float);

  for (i = 0; i < count - 1; i++) {
    j = i * 2;
    nw[j]     = inp[i] * 0.6666666667 + inp[i + 1] * 0.3333333333;
    nw[j + 1] = old[i];

    printf("old[i]: %f %f\n", old[i], glm_deg(old[i]));
  }

  free(old);
  otn->data = nw;
  ch->sampler->outTangentStride = ch->sampler->outTangentStride * 2;
}

GK_EXPORT
void
gkGenTangentKeysIfNeeded(GkChannel * __restrict ch) {
  uint32_t its, ots, itemc;

  itemc = ch->kv[0].rowCount;
  its   = ch->sampler->inTangentStride;
  ots   = ch->sampler->outTangentStride;

  /* 1D in tangents */
  if (its == itemc)
    gkGenInTangentKeys(ch);

  /* 1D out tangents */
  if (ots == itemc)
    gkGenOutTangentKeys(ch);

  ch->tangentsKeyGenerated = true;
}

//GK_EXPORT
//void
//gkGenTangentKeysIfNeeded(GkAnimation * __restrict anim) {
//  GkKeyFrameAnimation *kfa;
//  GkChannel           *ch;
//  uint32_t             its, ots, itemc, isReverse;
//
//  kfa       = (GkKeyFrameAnimation *)anim;
//  ch        = kfa->channel;
//  isReverse = anim->isReverse;
//
//  while (ch) {
//    itemc = ch->kv[isReverse].rowCount;
//    its   = ch->sampler->inTangentStride;
//    ots   = ch->sampler->outTangentStride;
//
//    if (ch->tangentsKeyGenerated)
//      goto nxt_ch;
//
//    /* 1D in tangents */
//    if (its == itemc)
//      gkGenInTangentKeys(ch);
//
//    /* 1D out tangents */
//    if (ots == itemc)
//      gkGenOutTangentKeys(ch);
//
//    ch->tangentsKeyGenerated = true;
//
//  nxt_ch:
//    ch = ch->next;
//  }
//}
