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
  GkPass  *pass;
  GkLight *currLight;
  float   *distances;
  mat4    *viewProj;
  int      splitc;
  float    near;
  float    far;
  GkSize   size;
} GkShadowMap;

typedef GkShadowMap* (*gkSetupShadowsFunc)(GkScene * __restrict scene,
                                           GkLight * __restrict light);
typedef void (*gkRenderShadowsFunc)(GkScene * __restrict scene,
                                    GkLight * __restrict light);

GkShadowMap*
gkSetupShadows(GkScene * __restrict scene,
               GkLight * __restrict light);

void
gkRenderShadows(GkScene * __restrict scene,
                GkLight * __restrict light);

#endif /* src_shadows_h */
