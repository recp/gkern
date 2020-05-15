/*
 * Copyright (c), Recep Aslantas.
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "../../include/gk/value.h"

GK_EXPORT
float
gkFloatValue(GkValue *val) {
  return val->val.floatValue;
}

GK_EXPORT
void
gkInitValue(GkValue * __restrict val,
            GkType               type,
            uint32_t             rowc,
            uint32_t             colc,
            uint32_t             itemsz) {
  val->type     = type;
  val->itemSize = itemsz;
  val->colc     = GLM_MAX(colc, 1);
  val->rowc     = GLM_MAX(rowc, 1);

  if (rowc > 1 || colc > 1) {
    val->val.p = malloc(itemsz * val->colc * val->rowc);
  } else {
    val->val.p = NULL;
  }
}

GK_EXPORT
void
gkInitValueAsFloat(GkValue *val, float fval) {
  val->type           = GKT_FLOAT;
  val->val.floatValue = fval;
  val->rowc           = 1;
  val->colc           = 1;
}

GK_EXPORT
void
gkInitValueAsVec3(GkValue *val, vec3 v) {
  val->type  = GKT_FLOAT3;
  val->val.p = realloc(val->val.p, sizeof(vec3));
  val->rowc  = 3;
  val->colc  = 1;
  glm_vec3_copy(v, val->val.p);
}

GK_EXPORT
void
gkInitValueAsVec4(GkValue *val, vec4 v) {
  val->type  = GKT_FLOAT4;
  val->val.p = realloc(val->val.p, sizeof(vec4));
  val->rowc  = 4;
  val->colc  = 1;
  glm_vec4_ucopy(v, val->val.p);
}

GK_EXPORT
void
gkInitValueAsFloatArray(GkValue *val, float *v, int32_t count) {
  float *dest;
  int    i;

  val->type  = GKT_FLOAT;
  val->val.p = realloc(val->val.p, sizeof(*v) * count);
  val->rowc  = count;
  val->colc  = 1;
  
  dest = val->val.p;

  for (i = 0; i < count; i++)
    dest[i] = v[i];
}

GK_EXPORT
void
gkInitValueAsMat3(GkValue *val, mat3 m) {
  val->type  = GKT_FLOAT3x3;
  val->val.p = realloc(val->val.p, sizeof(mat3));
  glm_mat3_copy(m, val->val.p);
}

GK_EXPORT
void
gkInitValueAsMat4(GkValue *val, mat4 m) {
  val->type  = GKT_FLOAT4x4;
  val->val.p = realloc(val->val.p, sizeof(mat4));
  glm_mat4_ucopy(m, val->val.p);
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

  val        = malloc(sizeof(*val));
  val->val.p = NULL;

  gkInitValueAsVec3(val, v);

  return val;
}

GK_EXPORT
GkValue*
gkValueFromVec4(vec4 v) {
  GkValue *val;

  val        = malloc(sizeof(*val));
  val->val.p = NULL;

  gkInitValueAsVec4(val, v);

  return val;
}

GK_EXPORT
GkValue*
gkValueFromMat4(mat4 m) {
  GkValue *val;

  val        = malloc(sizeof(*val));
  val->val.p = NULL;

  gkInitValueAsMat4(val, m);

  return val;
}
