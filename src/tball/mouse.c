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

#include "../common.h"
#include "../../include/gk/gk.h"
#include "../../include/gk/trackball.h"
#include "tball.h"
#include "mouse.h"

void
gkTrackballMouse(void         *sender,
                 GkScene      *scene,
                 GkMouseButton button,
                 GkMouseState  state,
                 GkPoint       point) {
  gkTrackballMouseWs(&(GkMouseEventStruct) {
    sender, scene, button, state, point
  });
}

void
gkTrackballMouseWs(GkMouseEventStruct *event) {
  GkTrackball *tball;
  GkScene     *scene;
  GkTransform *scnTrans;

  tball = event->sender;
  if (event->button != GK_MOUSE_LEFT)
    return;

  scene    = tball->scene;
  scnTrans = scene->trans;

  if (!scene->camera)
    return;

  switch (event->state) {
    case GK_MOUSE_DOWN:
      tball->start  = event->point;
      tball->moving = true;

      if (tball->cb)
        tball->cb(tball, GK_TRACKBALL_EVENT_BEGIN);

      gkTrackballVec(tball, tball->start, tball->from);

      break;
    case GK_MOUSE_MOVE:
      if (tball->moving) {
        vec3  axis, to;
        float angle;

        gkTrackballVec(tball, event->point, to);
        glm_vec3_cross(tball->from, to, axis);

        angle = glm_vec3_angle(tball->from, to) * tball->velocity;

        glm_vec3_rotate_m4(scene->camera->world, axis, axis);
        glm_rotate_atm(tball->trans, tball->center, angle, axis);
        glm_mat4_mul(tball->trans, scnTrans->local, scnTrans->world);

        scnTrans->flags &= ~GK_TRANSF_WORLD_ISVALID;
        scene->flags    |=  GK_SCENEF_RENDER;
        tball->moved     = true;
      }
      break;
    case GK_MOUSE_UP: {
      if (tball->moving && tball->moved) {
        tball->moving = tball->moved = false;

        glm_mat4_mul(tball->trans, scnTrans->local, scnTrans->local);

        scnTrans->flags &= ~GK_TRANSF_WORLD_ISVALID;
        scene->flags    |=  GK_SCENEF_RENDER;

        if (tball->cb)
          tball->cb(tball, GK_TRACKBALL_EVENT_END);
        break;
      }
    }
  }
}
