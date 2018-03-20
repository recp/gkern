/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
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
