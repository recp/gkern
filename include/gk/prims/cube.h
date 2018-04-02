/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_cube_h
#define gk_cube_h

#include "../common.h"
#include "../model.h"

struct GkScene;

void
gkDrawBBox(struct GkScene * __restrict scene,
           GkBBox                      bbox,
           mat4                        world);

void
gkInitCube(void);

void
gkReleaseCube(void);

#endif /* gk_cube_h */
