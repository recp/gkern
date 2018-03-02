/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef src_transp_common_h
#define src_transp_common_h

#include "../../include/gk/gk.h"
#include "../../include/gk/transparent.h"
#include "../../include/gk/prims/builtin-prim.h"

typedef struct GkTranspBase {
  GkTranspTechnType type;
} GkTranspBase;

_gk_hide
GkRenderPathFn
gkTranpRenderFunc(void);

#endif /* src_transp_common_h */
