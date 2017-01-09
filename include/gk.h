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
  GkMatrix       cachedMatrix;
  uint32_t       cachedMatrixIsValid;
  GkMatrix      *matrix;
  GkModelBase   *model;
  GkModelInst   *instance;
  struct GkNode *next;
  struct GkNode *chld;
  struct GkNode *nodeInst;
} GkNode;

typedef struct GkNodeInst {
  GkMatrix       cachedMatrix;
  uint32_t       cachedMatrixIsValid;
  GkMatrix      *matrix;
  GkNode        *instance;
  struct GkNode *next;
} GkNodeInst;

typedef struct GkScene {
  GkNode   *rootNode;
  GkMatrix *mat;
  GLint     matrixLoc;
  uint32_t  matrixIsValid;
} GkScene;

GkModelInst *
gkMakeInstance(GkModelBase *model, GkMatrix *matrix);

void
gkUniformModelMatrix(GkModelBase *modelBase);

void
gkUniformInstanceMatrix(GkModelInst *instance);

void
gkRenderInstance(GkModelInst *instance,
                 GkMatrix    *parentTrans);

void
gkRenderModel(GkModelBase *model,
              GkMatrix    *parentTrans);

void
gkRenderNode(GkNode   *node,
             GkMatrix *parentTrans);

void
gkRenderScene(GkScene * scene);

#ifdef __cplusplus
}
#endif
#endif /* gk_h */
