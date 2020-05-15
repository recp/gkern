/*
 * Copyright (c), Recep Aslantas.
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "../../include/gk/value.h"

GK_EXPORT
void
gkValueLerp(GkValue *from, GkValue *to, float t, GkValue *dest) {
  if (!dest->val.p && from->type != GKT_FLOAT)
    dest->val.p = malloc(from->itemSize * from->colc * from->rowc);

  if (dest->rowc == 1) {
    dest->val.floatValue = glm_lerp(from->val.floatValue,
                                    to->val.floatValue,
                                    t);
  } else {
    float *fv, *tv, *dv;
    int    i;
    
    fv = from->val.p;
    tv = to->val.p;
    dv = dest->val.p;

    for (i = 0; i < from->rowc; i++)
      dv[i] = glm_lerp(fv[i], tv[i], t);
  }

  dest->type = to->type;
}

GK_EXPORT
void
gkValueCopy(GkValue *src, GkValue *dest) {
  if (!dest->val.p && src->type != GKT_FLOAT)
    dest->val.p = malloc(src->itemSize * src->colc * src->rowc);

  if (dest->rowc == 1) {
    dest->val.floatValue =src->val.floatValue;
  } else {
    float *fv, *dv;
    int    i;
    
    fv = src->val.p;
    dv = dest->val.p;

    for (i = 0; i < src->rowc; i++)
      dv[i] = fv[i];
  }

  dest->type = src->type;
}

GK_EXPORT
void
gkValueSub(GkValue *a, GkValue *b, GkValue *dest) {
  if (!b || (b->type != GKT_FLOAT && !b->val.p)) {
    gkValueCopy(a, dest);
    return;
  }

  if (!dest->val.p && a->type != GKT_FLOAT)
    dest->val.p = malloc(a->itemSize * a->colc * a->rowc);

  if (dest->rowc == 1) {
    dest->val.floatValue = a->val.floatValue - b->val.floatValue;
  } else {
    float *av, *bv, *dv;
    int    i;
    
    av = a->val.p;
    bv = b->val.p;
    dv = dest->val.p;

    for (i = 0; i < a->rowc; i++)
      dv[i] = av[i] - bv[i];
  }

  dest->type = a->type;
}

GK_EXPORT
void
gkVectorSubf(float * __restrict a,
             float * __restrict b,
             float * __restrict dest,
             int                count) {
  while (count-- > 0)
    dest[count] = a[count] - b[count];
}

GK_EXPORT
void
gkValueFree(GkValue *a) {
  if (a->val.p)
    free(a->val.p);

  free(a);
}
