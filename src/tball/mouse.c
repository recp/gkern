/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
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
        glm_vec_cross(tball->from, to, axis);

        angle = glm_vec_angle(tball->from, to) * tball->velocity;

        glm_vec_rotate_m4(scene->camera->world, axis, axis);
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
