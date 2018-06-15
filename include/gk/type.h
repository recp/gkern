/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef type_h
#define type_h

#include "common.h"

typedef enum GkType {
  GKT_UNKNOWN = 0,
  GKT_INT32   = 1,
  GKT_UINT32  = 2,
  GKT_INT16   = 3,
  GKT_UINT16  = 4,
  GKT_INT8    = 5,
  GKT_UINT8   = 6,
  GKT_FLOAT   = 7,
  GKT_DOUBLE  = 8,
  GKT_FLOAT3  = 9
} GkType;

#endif /* type_h */
