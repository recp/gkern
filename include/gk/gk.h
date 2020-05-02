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

#ifndef gk_h
#define gk_h
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include "context.h"
#include "shader.h"
#include "program.h"
#include "light.h"
#include "material.h"
#include "bbox.h"
#include "geom-types.h"
#include "image.h"
#include "model.h"
#include "node.h"
#include "transform.h"
#include "camera.h"
#include "scene.h"
#include "render.h"

#include <ds/rb.h>

#include <stdlib.h>
#include <cglm/cglm.h>

#define GK_DRAW_ELEMENTS (1 << 0)
#define GK_DRAW_ARRAYS   (1 << 1)
#define GK_COMPLEX       (1 << 2)

void
gkReshape(GkScene *scene, GkRect rect);

GK_EXPORT
void
gkGetDepthForPoint(vec3 point);

#ifdef __cplusplus
}
#endif
#endif /* gk_h */
