/*
 * Copyright (c), Recep Aslantas.
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "../../include/gk/value.h"

GK_EXPORT
float
gkFloatValue(GkValue *val) {
  return val->s32.floatValue;
}

GK_EXPORT
GkValue*
gkValueFromFloat(float fval) {
  GkValue *val;

  val                 = malloc(sizeof(*val));
  val->type           = GKT_FLOAT;
  val->s32.floatValue = fval;

  return val;
}

GK_EXPORT
GkValue*
gkValueFromVec3(vec3 v) {
  GkValue *val;

  val       = malloc(sizeof(*val));
  val->type = GKT_FLOAT3;
  val->val  = malloc(sizeof(vec3));

  glm_vec_copy(v, val->val);

  return val;
}
