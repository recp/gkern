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
  GK_SCENEF_NONE          = 0,
  GK_SCENEF_DRAW_BBOX     = 1 << 0,
  GK_SCENEF_UPDT_VIEW     = 1 << 1,
  GK_SCENEF_UPDT_PROJ     = 1 << 2,
  GK_SCENEF_UPDT_LIGHTS   = 1 << 3,
  GK_SCENEF_ONCE          = 1 << 4,
  GK_SCENEF_RENDERING     = 1 << 5,
  GK_SCENEF_RENDERED      = 1 << 6,
  GK_SCENEF_RENDER        = 1 << 7,
  GK_SCENEF_UPDT_VIEWPROJ = GK_SCENEF_UPDT_VIEW | GK_SCENEF_UPDT_PROJ,
  GK_SCENEF_NEEDS_RENDER  = GK_SCENEF_RENDER
                            | GK_SCENEF_UPDT_VIEW
                            | GK_SCENEF_UPDT_PROJ
                            | GK_SCENEF_UPDT_LIGHTS,
  GK_SCENEF_INIT          = GK_SCENEF_NEEDS_RENDER
} GkSceneFlags;

GK_MAKE_C_ENUM(GkSceneFlags)

typedef struct GkScene {
  GkCamera    *camera;
  GkTransform *trans; /* free camera */
  GkNode      *rootNode;
  GkProgInfo  *pinfo;
  GkLightRef  *lights;
  GkBBox      *bbox;
  GkRect       vrect;
  uint32_t     lightCount;
  uint32_t     lastLightIndex;
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

GkPrimInst*
gkMakePrimInst(GkModelInst *modelInst,
               GkPrimitive *prim);

void
gkUniformMatrix(GkModelInst *modelInst);

void
gkPrepModel(GkScene     *scene,
            GkModelInst *modelInst,
            GkTransform *ptr,
            GkProgInfo  *pprog);

void
gkRenderModel(GkScene     *scene,
              GkModelInst *modelInst,
              GkTransform *ptr,
              GkProgInfo  *pprog);

void
gkPrepNode(GkScene     *scene,
           GkNode      *node,
           GkTransform *ptr,
           GkProgInfo  *pprog);

void
gkRenderNode(GkScene     *scene,
             GkNode      *node,
             GkTransform *ptr,
             GkProgInfo  *pprog);

void
gkRenderScene(GkScene * scene);

#ifdef __cplusplus
}
#endif
#endif /* gk_h */
