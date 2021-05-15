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

#ifndef gk_render_h
#define gk_render_h
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

struct GkLight;

void
gkPrepMaterial(GkScene     *scene,
               GkGeometryInst *modelInst);

void
gkPrepModel(GkScene     *scene,
            GkGeometryInst *modelInst,
            GkTransform *ptr);

void
gkRenderModel(GkScene     *scene,
              GkGeometryInst *modelInst);

void
gkRnModelNoMatOPass(GkScene     *scene,
                    GkGeometryInst *modelInst);

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
