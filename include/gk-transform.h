/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_transform_h
#define gk_transform_h

#include <cglm.h>

GkPoint
gk_project2d(GkRect rect, mat4 mvp, vec3 v);

#endif /* gk_transform_h */
