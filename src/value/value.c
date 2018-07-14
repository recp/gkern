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
void
gkInitValueAsFloat(GkValue *val, float fval) {
  val->type           = GKT_FLOAT;
  val->s32.floatValue = fval;
}

GK_EXPORT
void
gkInitValueAsVec3(GkValue *val, vec3 v) {
  val->type = GKT_FLOAT3;
  val->val  = realloc(val->val, sizeof(vec3));
  glm_vec_copy(v, val->val);
}

GK_EXPORT
void
gkInitValueAsVec4(GkValue *val, vec4 v) {
  val->type = GKT_FLOAT4;
  val->val  = realloc(val->val, sizeof(vec4));
  glm_vec4_ucopy(v, val->val);
}

GK_EXPORT
GkValue*
gkValueFromFloat(float fval) {
  GkValue *val;

  val = malloc(sizeof(*val));
  gkInitValueAsFloat(val, fval);

  return val;
}

GK_EXPORT
GkValue*
gkValueFromVec3(vec3 v) {
  GkValue *val;

  val      = malloc(sizeof(*val));
  val->val = NULL;

  gkInitValueAsVec3(val, v);

  return val;
}
