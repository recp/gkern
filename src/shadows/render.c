/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "shadows.h"
#include "render.h"
#include "../render/realtime/rn_prim.h"

void
gkRenderShadowMap(GkScene     * __restrict scene,
                  GkShadowMap * __restrict sm,
                  GkModelInst * __restrict modelInst,
                  GkProgram   * __restrict prog,
                  int                      split) {
  mat4         mvp;
  GkModel     *model;
  GkPrimitive *primi;
  vec4        *world;

  model = modelInst->model;
  world = modelInst->trans->world;

  glm_mul(sm->viewProj[split], world, mvp);
  gkUniformMat4(prog->mvpi, mvp);

  /* render */
  primi = model->prim;
  while (primi) {
    glBindVertexArray(primi->vao);
    gkRenderPrim(scene, primi);
    primi = primi->next;
  }

  /* reset the state */
  glBindVertexArray(0);
}
