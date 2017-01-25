/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_geom_types_h
#define gk_geom_types_h

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
#endif

GkPoint
gk_rect_center(GkRect rect);

#endif /* gk_geom_types_h */
