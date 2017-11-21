/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef impl_camera_h
#define impl_camera_h

#include "../../include/gk/camera.h"

#include <stdint.h>

typedef struct GkCameraImpl {
  GkCamera  pub;
  uint32_t  transfSlot;
} GkCameraImpl;

#endif /* impl_camera_h */
