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

#ifndef gk_trackball_h
#define gk_trackball_h

#include "common.h"
#include "geom-types.h"
#include "mouse.h"

#include <cglm/cglm.h>
#include <stdbool.h>

struct GkScene;
struct GkTrackball;
struct GkNode;

typedef enum GkTrackballEventType {
  GK_TRACKBALL_EVENT_BEGIN = 0,
  GK_TRACKBALL_EVENT_END   = 1
} GkTrackballEventType;

typedef void (*GkTrackballEventCb)(struct GkTrackball *tball,
                                   GkTrackballEventType event);

typedef struct GkTrackball {
  void                  *data;
  struct GkScene        *scene;
  struct GkNode         *node;
  vec3                  *bbox;      /* attached node's bounding box  */
  GkTransform           *nodeTrans; /* attached node's transform     */
  GkMouseEvent           mouse;
  GkMouseEventWithStruct mouse2;
  GkTrackballEventCb     cb;
  GkPoint                start;
  vec3                   from;
  float                 *center;
  mat4                   trans;
  float                  velocity;
  bool                   enabled;
  bool                   moving;
  bool                   moved;
} GkTrackball;

GkTrackball*
gkTrackballAlloc(void);

void
gkTrackballAttach(GkTrackball * __restrict tball,
                  GkScene     * __restrict scene,
                  GkNode      * __restrict node,
                  vec3                     pivot);

void
gkTrackballFree(GkTrackball *tball);

#endif /* gk_trackball_h */
