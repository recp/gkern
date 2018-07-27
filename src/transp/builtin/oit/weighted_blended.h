/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef transp_weighted_blended_h
#define transp_weighted_blended_h

#include "../../../../include/gk/gk.h"
#include "../../../../include/gk/pass.h"
#include "../../transp_common.h"

typedef struct GkTranspWeightedBlended {
  GkTranspBase base;
  GkPass      *opaquePass;
  GkPass      *transpPass;
  GkPass      *composPass;
  GkPipeline   *composProg;
} GkTranspWeightedBlended;

void
gkTranspWeightedBlendedInit(GkScene * __restrict scene);

void
gkTranspWeightedBlended(GkScene * __restrict scene);

#endif /* transp_weighted_blended_h */
