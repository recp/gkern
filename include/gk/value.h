/*
 * This file is part of the gk project (https://github.com/recp/gk)
 * Copyright (c) Recep Aslantas.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef gk_value_h
#define gk_value_h
#ifdef __cplusplus
extern "C" {
#endif

#include "type.h"
#include <stdint.h>

typedef union GkFloatOrPointer {
  float  s32;
  float *p;
} GkFloatOrPointer;

typedef union GkValue32 {
  int32_t intValue;
  float   floatValue;
} GkValue32;

typedef struct GkValue {
  void     *val;
  uint32_t  itemSize;
  uint32_t  rowCount;
  uint32_t  colCount;
  GkType    type;
  GkValue32 s32;
  bool      initialized;
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
gkInitValue(GkValue * __restrict val,
            GkType               type,
            uint32_t             rowc,
            uint32_t             colc,
            uint32_t             itemsz);

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
gkVectorSubf(float *a, float *b, float *dest, int count);

GK_EXPORT
void
gkValueFree(GkValue *a);

#ifdef __cplusplus
}
#endif
#endif /* gk_value_h */
