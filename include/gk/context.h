/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef context_h
#define context_h

#include "common.h"

struct RBTree;
struct GkProgInfo;
struct FList;
struct GkGPUStates;

typedef struct GkContext {
  struct FList       *states; /* GPU state caches */
  struct RBTree      *mdltree;
  struct GkProgInfo  *pinfo;  /* default program  */
  struct GkGPUStates *currState;
} GkContext;

#endif /* context_h */
