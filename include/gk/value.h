/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_value_h
#define gk_value_h
#ifdef __cplusplus
extern "C" {
#endif

#include "type.h"
#include <stdint.h>

typedef enum GkValueType {
  GK_VALUE_FLOAT  = 0,
  GK_VALUE_FLOAT3 = 1
} GkValueType;

typedef union GkValue32 {
  int32_t intValue;
  float   floatValue;
} GkValue32;

typedef struct GkValue {
  void     *val;
  size_t    valsz;
  GkType    type;
  GkValue32 s32;
} GkValue;

GK_EXPORT
float
gkFloatValue(GkValue *val);

GK_EXPORT
GkValue*
gkValueFromFloat(float fval);

GK_EXPORT
GkValue*
gkValueFromVec3(vec3 v);

GK_EXPORT
GkValue*
gkValueFromVec4(vec4 v);

GK_EXPORT
GkValue*
gkValueFromMat4(mat4 m);

GK_EXPORT
void
gkInitValueAsFloat(GkValue *val, float fval);

GK_EXPORT
void
gkInitValueAsVec3(GkValue *val, vec3 v);

GK_EXPORT
void
gkInitValueAsVec4(GkValue *val, vec4 v);

GK_EXPORT
void
gkInitValueAsMat4(GkValue *val, mat4 m);

GK_EXPORT
void
gkValueLerp(GkValue *from, GkValue *to, float t, GkValue *dest);

GK_EXPORT
void
gkValueCopy(GkValue *src, GkValue *dest);

GK_EXPORT
void
gkValueSub(GkValue *a, GkValue *b, GkValue *dest);

GK_EXPORT
void
gkValueMul(GkValue *a, GkValue *b, GkValue *dest);

GK_EXPORT
void
gkValueFree(GkValue *a);

#ifdef __cplusplus
}
#endif
#endif /* gk_value_h */
