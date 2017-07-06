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
           mat4 world,
           vec3 min,
           vec3 max);

void
gkInitCube(void);

void
gkReleaseCube(void);

#endif /* gk_cube_h */
