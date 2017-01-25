/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_tball_mouse_h
#define gk_tball_mouse_h

#include "../../include/gk.h"
#include "../../include/gk-trackball.h"

void
gk_tball_mouse_ws(GkMouseEventStruct *event);

void
gk_tball_mouse(void         *sender,
               GkScene      *scene,
               GkMouseButton button,
               GkMouseState  state,
               GkPoint       point);

#endif /* gk_tball_mouse_h */
