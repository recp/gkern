/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef shadows_h
#define shadows_h

#include "common.h"

struct GkScene;
struct GkPassOut;

/* Shadow technique type */

typedef enum GkShadowTechnType {
  GK_SHADOW_AUTO               = 0,
  GK_SHADOW_DISABLED           = 1,
  GK_SHADOW_BASIC_SHADOWMAP    = 1,
  GK_SHADOW_COLORED_STOCHASTIC = 2,
  GK_SHADOW_DEFAULT            = GK_SHADOW_DISABLED
} GkShadowTechnType;

GK_EXPORT
void
gkSetShadowTechn(GkShadowTechnType techn);

GK_EXPORT
GkShadowTechnType
gkShadowTechn(void);

void
gkRenderShadowMapTo(GkScene          * __restrict scene,
                    struct GkPassOut * __restrict output);

#endif /* transparency_h */
