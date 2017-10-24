/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../include/gk/gk.h"
#include "../../include/gk/trackball.h"
#include "tball.h"
#include "tball_mouse.h"

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

      if (tball->cb)
        tball->cb(tball, GK_TRACKBALL_EVENT_BEGIN);
      break;
    case GK_MOUSE_MOVE:
      if (tball->moving == true) {
        mat4   rot = GLM_MAT4_IDENTITY_INIT;
        vec3   axis, from, to, tran;
        versor q;
        float  angle;

        gk_tall_vec(tball, tball->start, from);
        gk_tall_vec(tball, event->point, to);
        glm_vec_cross(from, to, axis);

        angle = acosf(fminf(1.0f, glm_vec_dot(from, to))) * tball->velocity;

        glm_mat4_mulv3(scene->camera->world, axis, axis);
        glm_quatv(q, angle, axis);
        glm_quat_normalize(q);

        /* rotate around center */
        glm_vec_sub(tball->bbox->center,
                    tball->node->trans->local[3],
                    tran);

        glm_quat_mat4(q, tball->trans);
        glm_translate(rot, tran);
        glm_mat4_mul(rot, tball->trans, tball->trans);

        glm_vec_flipsign(tran);
        glm_translate(tball->trans, tran);

        glm_mat4_mul(tball->trans,
                     scene->trans->local,
                     scene->trans->world);
        scene->trans->flags &= ~GK_TRANSF_WORLD_ISVALID;
        scene->flags |= GK_SCENEF_RENDER;
      }
      break;
    case GK_MOUSE_UP: {
      glm_mat4_mul(tball->trans,
                   scene->trans->local,
                   scene->trans->local);
      scene->trans->flags &= ~GK_TRANSF_WORLD_ISVALID;
      scene->flags |= GK_SCENEF_RENDER;

      if (tball->cb)
        tball->cb(tball, GK_TRACKBALL_EVENT_END);
      break;
    }
  }
}
