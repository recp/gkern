/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef src_shadows_h
#define src_shadows_h

#include "../../include/gk/gk.h"
#include "../../include/gk/shadows.h"
#include "../../include/gk/pass.h"

typedef struct GkShadowMap {
  GkPass  *shadowPass;
  GkLight *currLight;
  float    offset;
} GkShadowMap;

typedef GkShadowMap* (*gkSetupShadowsFunc)(GkScene * __restrict scene);
typedef void (*gkRenderShadowsFunc)(GkScene * __restrict scene,
                                    GkLight * __restrict light);

GkShadowMap*
gkSetupShadows(GkScene * __restrict scene);

void
gkRenderShadows(GkScene * __restrict scene,
                GkLight * __restrict light);

#endif /* src_shadows_h */
