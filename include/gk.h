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

#include <stdlib.h>
#include <cglm.h>

#define GK_DRAW_ELEMENTS (1 << 0)
#define GK_DRAW_ARRAYS   (1 << 1)
#define GK_COMPLEX       (1 << 2)

#include "gk-model.h"

typedef struct GkNode {
  GkModelBase   *model;
  GkModelInst   *instance;
  struct GkNode *next;
  struct GkNode *chld;
} GkNode;

typedef struct GkScene {
  GkNode *rootNode;
} GkScene;

GkModelInst *
gkMakeInstance(GkModelBase *model, GkMatrix *matrix);

void
gkUniformModelMatrix(GkModelBase *modelBase);

void
gkUniformInstanceMatrix(GkModelInst *instance);

void
gkRenderInstance(GkModelInst *instance);

void
gkRenderModel(GkModelBase *model);

void
gkRenderNode(GkNode * node);

void
gkRenderScene(GkScene * scene);

#ifdef __cplusplus
}
#endif
#endif /* gk_h */
