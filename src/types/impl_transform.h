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

#ifndef impl_transform_h
#define impl_transform_h

#include "impl_camera.h"
#include "impl_scene.h"
#include "../../include/gk/transform.h"
#include "../../include/gk/scene.h"

#include <stdint.h>
#include <ds/forward-list.h>

typedef struct GkFinalTransform {
  mat4     mvp;  /* model view projection matrix */
  mat4     mv;   /* model view matrix            */
  mat4     nm;   /* normal matrix                */
} GkFinalTransform;

typedef struct GkTransformImpl {
  GkTransform        pub;
  uint32_t           refc;
  uint32_t           ftrc;
  GkFinalTransform **ftr;    /* cached transform[s] and infos per camera     */
} GkTransformImpl;

void
gkUniformTransform(struct GkPipeline * __restrict prog,
                   GkTransform      * __restrict trans,
                   struct GkCamera  * __restrict cam);

void
gkCalcFinalTransf(struct GkScene * __restrict scene,
                  GkCamera       * __restrict cam,
                  GkTransform    * __restrict tr);

void
gkCalcViewTransf(struct GkScene * __restrict scene,
                 GkCamera       * __restrict cam,
                 GkTransform    * __restrict tr);
GK_EXPORT
void
gkResizeTransform(struct GkScene  * __restrict scene,
                  GkTransformImpl * __restrict trans);

GK_INLINE
GkFinalTransform*
gkFinalTransform(GkTransform * __restrict transf,
                 GkCamera    * __restrict cam) {
  GkTransformImpl *transfImpl;
  GkCameraImpl    *camImpl;

  transfImpl = (GkTransformImpl *)transf;
  camImpl    = (GkCameraImpl *)cam;

  if (camImpl->transfSlot & (1 << 30) || !transfImpl->ftr)
    return NULL;

  return transfImpl->ftr[camImpl->transfSlot];
}

GK_INLINE
GkFinalTransform*
gkSetFinalTransform(struct GkScene * __restrict scene,
                    GkTransform    * __restrict transf,
                    GkCamera       * __restrict cam) {
  GkTransformImpl  *transfImpl;
  GkCameraImpl     *camImpl;
  GkFinalTransform *ftr;
  GkSceneImpl      *sceneImpl;

  sceneImpl  = (GkSceneImpl *)scene;
  transfImpl = (GkTransformImpl *)transf;
  camImpl    = (GkCameraImpl *)cam;

  gkResizeTransform(scene, transfImpl);
  if (sceneImpl->transfCacheSlots->count == 0)
    return NULL;

  if (!(ftr = transfImpl->ftr[camImpl->transfSlot]))
    transfImpl->ftr[camImpl->transfSlot] = ftr = malloc(sizeof(*ftr));

  return ftr;
}

#endif /* impl_transform_h */
