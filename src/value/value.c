/*
 * Copyright (c), Recep Aslantas.
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "../../include/gk/value.h"

float
gkFloatValue(GkValue *val) {
  return val->s32.floatValue;
}

GkValue*
gkValueFromFloat(float fval) {
  GkValue *val;

  val = malloc(sizeof(*val));
  val->s32.floatValue = fval;

  return val;
}
