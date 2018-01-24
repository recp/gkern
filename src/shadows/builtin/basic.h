/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef basic_h
#define basic_h

#include "../../../include/gk/gk.h"
#include "../../../include/gk/scene.h"

#include "../shadows.h"

GkShadowMap*
gkSetupBasicShadowMap(GkScene * __restrict scene,
                      GkLight * __restrict light);

void
gkRenderBasicShadowMap(GkScene * __restrict scene,
                       GkLight * __restrict light);

#endif /* basic_h */
