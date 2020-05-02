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

#ifndef type_h
#define type_h

#include "common.h"

typedef enum GkType {
  GKT_UNKNOWN  = 0,
  GKT_INT32    = 1,
  GKT_UINT32   = 2,
  GKT_INT16    = 3,
  GKT_UINT16   = 4,
  GKT_INT8     = 5,
  GKT_UINT8    = 6,
  GKT_FLOAT    = 7,
  GKT_DOUBLE   = 8,
  GKT_FLOAT3   = 9,
  GKT_FLOAT4   = 10,
  GKT_FLOAT3x3 = 11,
  GKT_FLOAT4x4 = 12
} GkType;

#endif /* type_h */
