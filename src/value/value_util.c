/*
 * Copyright (c), Recep Aslantas.
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "../../include/gk/value.h"

GK_EXPORT
void
gkValueLerp(GkValue *from, GkValue *to, float t, GkValue *dest) {
  if (!from->val && from->type != GKT_FLOAT)
    dest->val = malloc(from->itemSize * from->colCount * from->rowCount);

  switch (to->type) {
    case GKT_FLOAT:
      dest->s32.floatValue = glm_lerp(from->s32.floatValue,
                                      to->s32.floatValue,
                                      t);
      break;
    case GKT_FLOAT3:
      glm_vec3_lerp(from->val, to->val, t, dest->val);
      break;
    case GKT_FLOAT4:
      glm_vec4_lerp(from->val, to->val, t, dest->val);
      break;
    default:
      break;
  }

  dest->type = to->type;
}

GK_EXPORT
void
gkValueCopy(GkValue *src, GkValue *dest) {
  if (!dest->val && src->type != GKT_FLOAT)
    dest->val = malloc(src->itemSize * src->colCount * src->rowCount);

  switch (src->type) {
    case GKT_FLOAT:
      dest->s32.floatValue = src->s32.floatValue;
      break;
    case GKT_FLOAT3:
      glm_vec3_copy(src->val, dest->val);
      break;
    case GKT_FLOAT4:
      glm_vec4_ucopy(src->val, dest->val);
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

  if (!dest->val && a->type != GKT_FLOAT)
    dest->val = malloc(a->itemSize * a->colCount * a->rowCount);

  switch (a->type) {
    case GKT_FLOAT:
      dest->s32.floatValue = a->s32.floatValue - b->s32.floatValue;
      break;
    case GKT_FLOAT3:
      glm_vec3_sub(a->val, b->val, dest->val);
      break;
    case GKT_FLOAT4:
      glm_vec4_sub(a->val, b->val, dest->val);
      break;
    default:
      break;
  }

  dest->type = a->type;
}

GK_EXPORT
void
gkVectorSubf(float *a, float *b, float *dest, int count) {
  while (count-- > 0)
    dest[count] = a[count] - b[count];
}

GK_EXPORT
void
gkValueFree(GkValue *a) {
  if (a->val)
    free(a->val);

  free(a);
}
