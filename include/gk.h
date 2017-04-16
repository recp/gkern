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

#include <stdlib.h>
#include <cglm.h>

#define GK_DRAW_ELEMENTS (1 << 0)
#define GK_DRAW_ARRAYS   (1 << 1)
#define GK_COMPLEX       (1 << 2)

typedef struct GkContext {
  RBTree     *mdltree;
  GkProgInfo *pinfo; /* top-level program */
} GkContext;

typedef enum GkSceneFlags {
  GK_SCENE_FLAGS_NONE      = 0,
  GK_SCENE_FLAGS_DRAW_BBOX = 1 << 0,
} GkSceneFlags;

typedef struct GkScene {
  mat4         v;
  mat4         vinv;
  mat4         pv;
  GkMatrix    *trans; /* free camera */
  GkNode      *rootNode;
  GkProgInfo  *pinfo;
  GkLightRef  *lights;
  GkBBox      *bbox;
  GkRect       vrect;
  uint32_t     lightCount;
  uint32_t     lastLightIndex;
  uint8_t      pvIsValid;
  uint8_t      vIsValid;
  uint8_t      lightsAreValid;
  GLenum       usage;
  GLuint       currentProgram;
  GkSceneFlags flags;
} GkScene;

GkContext*
gkContextNew(GkProgInfo * __restrict pinfo);

void
gkContextFree(GkContext *ctx);

GkModelInst*
gkMakeInstance(GkModel *model);

void
gkUniformMatrix(GkModelInst *modelInst);

void
gkRenderModel(GkScene     *scene,
              GkModelInst *modelInst,
              GkMatrix    *parentMat,
              GkProgInfo  *parentProg);

void
gkPrepNode(GkScene    *scene,
           GkNode     *node,
           GkMatrix   *pmat,
           GkProgInfo *pprog);

void
gkRenderNode(GkScene    *scene,
             GkNode     *node,
             GkMatrix   *parentMat,
             GkProgInfo *parentProg);

void
gkRenderScene(GkScene * scene);

#ifdef __cplusplus
}
#endif
#endif /* gk_h */
