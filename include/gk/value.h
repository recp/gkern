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

typedef union GkSimpleValue32 {
  int32_t intValue;
  float   floatValue;
} GkSimpleValue32;

typedef struct GkValue {
  void           *value;
  GkType          type;
  GkSimpleValue32 s32;
} GkValue;

float
gkFloatValue(GkValue *val);

GkValue*
gkValueFromFloat(float fval);

#ifdef __cplusplus
}
#endif
#endif /* gk_value_h */
