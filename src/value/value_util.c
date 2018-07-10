/*
 * Copyright (c), Recep Aslantas.
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "../../include/gk/value.h"

GK_EXPORT
void
gkValueLerp(GkValue *from, GkValue *to, float t, GkValue *dest) {
  switch (to->type) {
    case GKT_FLOAT:
      dest->s32.floatValue = glm_lerp(from->s32.floatValue,
                                      to->s32.floatValue,
                                      t);
      break;
    case GKT_FLOAT3:
      dest->val = realloc(dest->val, sizeof(vec3));
      glm_vec_lerp(from->val, to->val, t, dest->val);
      break;
    default:
      break;
  }

  dest->type = to->type;
}

GK_EXPORT
void
gkValueStep(GkValue *edge, GkValue *x, GkValue *dest) {
  switch (x->type) {
    case GKT_FLOAT:
      dest->s32.floatValue = glm_step(edge->s32.floatValue,
                                      x->s32.floatValue);
      break;
    case GKT_FLOAT3:
      dest->val = realloc(dest->val, sizeof(vec3));
      glm_vec_step(edge->val, x->val, dest->val);
      break;
    default:
      break;
  }

  dest->type = x->type;
}

GK_EXPORT
void
gkValueCopy(GkValue *src, GkValue *dest) {
  switch (src->type) {
    case GKT_FLOAT:
      dest->s32.floatValue = src->s32.floatValue;
      break;
    case GKT_FLOAT3:
      dest->val = realloc(dest->val, sizeof(vec3));
      glm_vec_copy(src->val, dest->val);
      break;
    default:
      break;
  }

  dest->type = src->type;
}

GK_EXPORT
void
gkValueSub(GkValue *a, GkValue *b, GkValue *dest) {
  if (!b || (b->type != GKT_FLOAT && !b->val)) {
    gkValueCopy(a, dest);
    return;
  }

  switch (a->type) {
    case GKT_FLOAT:
      dest->s32.floatValue = a->s32.floatValue - b->s32.floatValue;
      break;
    case GKT_FLOAT3:
      dest->val = realloc(dest->val, sizeof(vec3));
      glm_vec_sub(a->val, b->val, dest->val);
      break;
    default:
      break;
  }

  dest->type = a->type;
}

GK_EXPORT
void
gkValueFree(GkValue *a) {
  if (a->val)
    free(a->val);

  free(a);
}
