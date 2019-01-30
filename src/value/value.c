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
gkInitValue(GkValue * __restrict val,
            GkType               type,
            size_t               rowc,
            size_t               colc,
            size_t               itemsz) {
  val->type     = type;
  val->itemSize = itemsz;
  val->colCount = GLM_MAX(colc, 1);
  val->rowCount = GLM_MAX(rowc, 1);

  if (rowc > 1 || colc > 1) {
    val->val = malloc(itemsz * val->colCount * val->rowCount);
  } else {
    val->val = &val->s32;
  }
}

GK_EXPORT
void
gkInitValueAsFloat(GkValue *val, float fval) {
  val->type           = GKT_FLOAT;
  val->s32.floatValue = fval;
  val->rowCount = 1;
  val->colCount = 1;
  val->val = &val->s32.floatValue;
}

GK_EXPORT
void
gkInitValueAsVec3(GkValue *val, vec3 v) {
  val->type  = GKT_FLOAT3;
  val->val   = realloc(val->val, sizeof(vec3));
  val->rowCount = 3;
  val->colCount = 1;
  glm_vec3_copy(v, val->val);
}

GK_EXPORT
void
gkInitValueAsVec4(GkValue *val, vec4 v) {
  val->type  = GKT_FLOAT4;
  val->val   = realloc(val->val, sizeof(vec4));
  glm_vec4_ucopy(v, val->val);
}

GK_EXPORT
void
gkInitValueAsMat3(GkValue *val, mat3 m) {
  val->type  = GKT_FLOAT3x3;
  val->val   = realloc(val->val, sizeof(mat3));
  glm_mat3_copy(m, val->val);
}

GK_EXPORT
void
gkInitValueAsMat4(GkValue *val, mat4 m) {
  val->type  = GKT_FLOAT4x4;
  val->val   = realloc(val->val, sizeof(mat4));
  glm_mat4_ucopy(m, val->val);
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

GK_EXPORT
GkValue*
gkValueFromVec4(vec4 v) {
  GkValue *val;

  val      = malloc(sizeof(*val));
  val->val = NULL;

  gkInitValueAsVec4(val, v);

  return val;
}

GK_EXPORT
GkValue*
gkValueFromMat4(mat4 m) {
  GkValue *val;

  val      = malloc(sizeof(*val));
  val->val = NULL;

  gkInitValueAsMat4(val, m);

  return val;
}
