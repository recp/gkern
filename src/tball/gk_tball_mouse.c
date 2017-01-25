/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../include/gk.h"
#include "../../include/gk-trackball.h"
#include "gk_tball.h"
#include "gk_tball_mouse.h"

void
gk_tball_mouse(void         *sender,
               GkScene      *scene,
               GkMouseButton button,
               GkMouseState  state,
               GkPoint       point) {
  gk_tball_mouse_ws(&(GkMouseEventStruct) {
    sender,
    scene,
    button,
    state,
    point
  });
}

void
gk_tball_mouse_ws(GkMouseEventStruct *event) {
  GkTrackball *tball;
  GkScene     *scene;

  tball = event->sender;
  if (event->button != GK_MOUSE_LEFT)
    return;

  scene = tball->scene;
  switch (event->state) {
    case GK_MOUSE_DOWN:
      tball->start  = event->point;
      tball->moving = true;
      break;
    case GK_MOUSE_MOVE:
      if (tball->moving == true) {
        vec3   v1, v2, axis;
        versor q;
        float  angle;

        gk_tall_vec(tball, tball->start, v1);
        gk_tall_vec(tball, event->point, v2);
        glm_vec_cross(v1, v2, axis);

        angle = acosf(glm_vec_dot(v1, v2));

        glm_quatv(q, angle, axis);
        glm_quat_normalize(q);
        glm_quat_mat4(q, tball->trans);

        glm_mat4_mul(tball->trans,
                     scene->trans->matrix,
                     scene->trans->cmat);
        scene->trans->cmatIsValid = 0;
      }
      break;
    case GK_MOUSE_UP: {
      glm_mat4_mul(tball->trans,
                   scene->trans->matrix,
                   scene->trans->matrix);
      scene->trans->cmatIsValid = 0;
      break;
    }
  }
}
