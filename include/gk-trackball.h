/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_trackball_h
#define gk_trackball_h

#include "gk-geom-types.h"
#include "gk-mouse.h"

#include <cglm.h>
#include <stdbool.h>

struct GkScene;

typedef struct GkTrackball {
  struct GkScene        *scene;
  GkBBox                *bbox;   /* bounding box which is tball attached to */
  GkMatrix              *matrix; /* attached node's matrix */
  GkMouseEvent           mouse;
  GkMouseEventWithStruct mouse2;
  GkPoint                start;
  mat4                   trans;
  bool                   enabled;
  bool                   moving;
} GkTrackball;

GkTrackball*
gk_tball_new();

void
gk_tball_init(GkTrackball * __restrict tball,
              GkScene     * __restrict scene,
              GkNode      * __restrict node,
              GkBBox      * __restrict bbox);

void
gkTrackballFree(GkTrackball *tball);

void
gkTrackballReset(GkTrackball *tball);

#endif /* gk_trackball_h */
