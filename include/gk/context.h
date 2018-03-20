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
struct GkProgram;
struct FList;
struct GkGPUStates;
struct FList;

typedef struct GkContext {
  struct FList       *states;   /* GPU state caches */
  struct RBTree      *mdltree;
  struct GkGPUStates *currState;
  struct FList       *samplers; /* private          */
  uint32_t            availTexUnit;
} GkContext;

GK_EXPORT
GkContext*
gkAllocContext(void);

void
gkContextFree(GkContext *ctx);

#endif /* context_h */
