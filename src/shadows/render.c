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
                  GkPrimInst  * __restrict primInst,
                  GkProgram   * __restrict prog,
                  int                      split) {
  mat4         mvp;
  vec4        *world;
  GkPrimitive *prim;

  world = primInst->trans->world;
  prim  = primInst->prim;

  glm_mul(sm->viewProj[split], world, mvp);
  gkUniformMat4(prog->mvpi, mvp);

  glBindVertexArray(prim->vao);
  gkRenderPrim(scene, prim);

  /* reset the state */
  glBindVertexArray(0);
}
