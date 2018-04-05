/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_tball_h
#define gk_tball_h

#include "../../include/gk/gk.h"
#include "../../include/gk/trackball.h"
#include "../../include/gk/geom-types.h"

void
gkTrackballVec(GkTrackball * __restrict tball,
               GkPoint                   p,
               vec3                      vec);

#endif /* gk_tball_h */
