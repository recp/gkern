/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_mouse_h
#define gk_mouse_h

#include "gk-geom-types.h"

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
