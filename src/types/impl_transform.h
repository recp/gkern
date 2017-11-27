/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef impl_transform_h
#define impl_transform_h

#include "impl_camera.h"
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
  GkFinalTransform **ftr;  /* cached transform[s] and infos per camera */
} GkTransformImpl;

void
gkUniformTransform(struct GkProgram * __restrict prog,
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

  if (camImpl->transfSlot & (1 << 31) || !transfImpl->ftr)
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

  transfImpl = (GkTransformImpl *)transf;
  camImpl    = (GkCameraImpl *)cam;

  gkResizeTransform(scene, transfImpl);
  if (scene->_priv.transfCacheSlots->count == 0)
    return NULL;

  if (!(ftr = transfImpl->ftr[camImpl->transfSlot]))
    transfImpl->ftr[camImpl->transfSlot] = ftr = malloc(sizeof(*ftr));

  return ftr;
}


#endif /* impl_transform_h */
