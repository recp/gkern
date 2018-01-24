/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef csm_h
#define csm_h

#include "../../../include/gk/gk.h"
#include "../../../include/gk/scene.h"

#include "../shadows.h"

GkShadowMap*
gkSetupShadowMapCSM(GkScene * __restrict scene,
                    GkLight * __restrict light);

void
gkRenderShadowMapCSM(GkScene * __restrict scene,
                     GkLight * __restrict light);

#endif /* csm_h */
