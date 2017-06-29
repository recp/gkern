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

#include "gk-common.h"
#include "gk-shader.h"
#include "gk-program.h"
#include "gk-light.h"
#include "gk-material.h"
#include "gk-bbox.h"
#include "gk-geom-types.h"
#include "gk-image.h"
#include "gk-model.h"
#include "gk-rb.h"
#include "gk-node.h"
#include "gk-transform.h"
#include "gk-camera.h"
#include "gk-scene.h"
#include "gk-render.h"

#include <stdlib.h>
#include <cglm.h>

#define GK_DRAW_ELEMENTS (1 << 0)
#define GK_DRAW_ARRAYS   (1 << 1)
#define GK_COMPLEX       (1 << 2)

typedef struct GkContext {
  RBTree     *mdltree;
  GkProgInfo *pinfo; /* top-level program */
} GkContext;

GkContext*
gkContextNew(GkProgInfo * __restrict pinfo);

void
gkContextFree(GkContext *ctx);

#ifdef __cplusplus
}
#endif
#endif /* gk_h */
