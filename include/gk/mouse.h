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

#ifndef gk_mouse_h
#define gk_mouse_h

#include "common.h"
#include "geom-types.h"

typedef enum GkMouseState {
  GK_MOUSE_DOWN = 1,
  GK_MOUSE_UP   = 2,
  GK_MOUSE_MOVE = 3
} GkMouseState;

typedef enum GkMouseButton {
  GK_MOUSE_LEFT   = 1,
  GK_MOUSE_RIGHT  = 2,
  GK_MOUSE_MIDDLE = 3
} GkMouseButton;

typedef struct GkMouseEventStruct {
  void         *sender;
  GkScene      *scene;
  GkMouseButton button;
  GkMouseState  state;
  GkPoint       point;
} GkMouseEventStruct;

typedef void (*GkMouseEvent)(void         *sender,
                             GkScene      *scene,
                             GkMouseButton button,
                             GkMouseState  state,
                             GkPoint       point);

typedef void (*GkMouseEventWithStruct)(GkMouseEventStruct *event);

#endif /* gk_mouse_h */
