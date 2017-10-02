/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_pass_h
#define gk_pass_h

#include "common.h"
#include "program.h"
#include "material.h"

#include <ds/forward-list.h>
#include <ds/rb.h>

typedef struct GkPass {
  GkProgInfo    *pinfo;
  RBTree        *states;
  struct GkPass *next;
} GkPass;

GK_EXPORT
GkPass*
gkGetOrCreatPass(GkMaterial *mat);

#endif /* gk_pass_h */
