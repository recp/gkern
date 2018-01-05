/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_render_h
#define gk_render_h
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

struct GkLight;

void
gkPrepMaterial(GkScene     *scene,
               GkModelInst *modelInst);

void
gkPrepModel(GkScene     *scene,
            GkModelInst *modelInst,
            GkTransform *ptr);

void
gkRenderModel(GkScene     *scene,
              GkModelInst *modelInst);

void
gkRnModelNoMatOPass(GkScene     *scene,
                    GkModelInst *modelInst);

void
gkRnModelForShadowMap(GkScene     * __restrict scene,
                      GkModelInst * __restrict modelInst,
                      GkProgram   * __restrict prog);

void
gkPrepNode(GkScene     *scene,
           GkNode      *node,
           GkTransform *ptr);

void
gkRenderNode(GkScene     *scene,
             GkNode      *node,
             GkTransform *ptr);

void
gkRenderScene(GkScene * scene);

GK_EXPORT
void
gkRenderShadows(GkScene * __restrict scene,
                struct GkLight * __restrict light);

GK_EXPORT
void
gkScenePerLightRenderPath(GkScene * __restrict scene);

GK_EXPORT
void
gkModelPerLightRenderPath(GkScene * __restrict scene);

#ifdef __cplusplus
}
#endif
#endif /* gk_render_h */
