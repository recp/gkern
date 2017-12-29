/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef plane_h
#define plane_h

#include <cglm/cglm.h>

typedef enum GkPlaneIndex {
  GK_PLANE_LEFT   = 0,
  GK_PLANE_RIGHT  = 1,
  GK_PLANE_BOTTOM = 2,
  GK_PLANE_TOP    = 3,
  GK_PLANE_NEAR   = 4,
  GK_PLANE_FAR    = 5
} GkPlaneIndex;

typedef vec4 GkPlane;

#endif /* plane_h */
