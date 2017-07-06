/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/gk/gk.h"
#include "../include/gk/geom-types.h"

GkPoint
gk_rect_center(GkRect rect) {
  GkPoint p;

  p.x = rect.origin.x + rect.size.w * 0.5f;
  p.y = rect.origin.y + rect.size.h * 0.5f;

  return p;
}
