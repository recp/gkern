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

  if (its == 0 || itemc == 0)
    return;

  /* 1D in tangents */
  if (its == itemc)
    gkGenInTangentKeys(ch);

  /* 1D out tangents */
  if (ots == itemc)
    gkGenOutTangentKeys(ch);

  ch->tangentsKeyGenerated = true;
}
