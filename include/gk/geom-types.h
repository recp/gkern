/*
 * This file is part of the gk project (https://github.com/recp/gk)
 * Copyright (c) Recep Aslantas.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef gk_geom_types_h
#define gk_geom_types_h
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

typedef struct GkPoint {
  float x;
  float y;
} GkPoint;

typedef struct GkSize {
  float w;
  float h;
} GkSize;

typedef struct GkRect {
  GkPoint origin;
  GkSize  size;
} GkRect;

#ifdef __OBJC__
CG_INLINE
GkPoint
gkPointCGPoint(CGPoint point) {
  GkPoint p; p.x = point.x; p.y = point.y; return p;
}

CG_INLINE
GkSize
gkSizeCGSize(CGSize size) {
  GkSize s; s.w = size.width; s.h = size.height; return s;
}

CG_INLINE
GkRect
gkRectCGRect(CGRect rect) {
  GkRect r;
  r.origin.x = rect.origin.x;
  r.origin.y = rect.origin.y;
  r.size.w   = rect.size.width;
  r.size.h   = rect.size.height;
  return r;
}

CG_INLINE
void
gkViewportCGRect(CGRect rect, vec4 viewport) {
  viewport[0] = rect.origin.x;
  viewport[1] = rect.origin.y;
  viewport[2] = rect.size.width;
  viewport[3] = rect.size.height;
}

#endif

GkPoint
gk_rect_center(GkRect rect);

#ifdef __cplusplus
}
#endif
#endif /* gk_geom_types_h */
