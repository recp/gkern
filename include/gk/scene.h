/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_scene_h
#define gk_scene_h
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include "renderpath.h"

struct GkPassOut;
struct FListItem;
struct GkScene;
struct GkLight;
struct GkContext;
struct GkMaterial;
struct GkModelInst;
struct GkNode;
struct GkProgram;
struct GkLightRef;
struct GkBBox;

typedef void (*GkRenderModelFn)(struct GkScene     *scene,
                                struct GkModelInst *modelInst,
                                struct GkTransform *ptr);

typedef enum GkSceneFlags {
  GK_SCENEF_NONE          = 0,
  GK_SCENEF_DRAW_BBOX     = 1 << 0,
  GK_SCENEF_UPDT_LIGHTS   = 1 << 3,
  GK_SCENEF_ONCE          = 1 << 4,
  GK_SCENEF_RENDERING     = 1 << 5,
  GK_SCENEF_RENDERED      = 1 << 6,
  GK_SCENEF_RENDER        = 1 << 7,
  GK_SCENEF_NEEDS_RENDER  = GK_SCENEF_RENDER | GK_SCENEF_UPDT_LIGHTS,
  GK_SCENEF_INIT          = GK_SCENEF_NEEDS_RENDER,
  GK_SCENEF_TRANSP        = 1 << 8,
  GK_SCENEF_SHADOWS       = 1 << 9,
  GK_SCENEF_PREPARED      = 1 << 10
} GkSceneFlags;

GK_MAKE_C_ENUM(GkSceneFlags)

typedef struct GkScenePrivateFields {
  struct GkContext  *ctx;
  struct FListItem  *transpPrims;
  struct GkLight    *forLight;
  void              *shadows;
  struct GkPass     *overridePass;     /* override all passes    */
  struct GkMaterial *overrideMaterial; /* override all materials */
  struct FList      *transfCacheSlots;
  GkRenderPathFn     rp;
  GkRenderPathType   rpath;
} GkScenePrivateFields;

typedef struct GkScene {
  GkScenePrivateFields _priv;
  GkCamera          *camera;
  GkTransform       *trans;  /* free camera */
  struct GkNode     *rootNode;
  struct GkLightRef *lights;
  struct GkBBox     *bbox;
  struct GkPassOut  *finalOutput; /* set NULL for default FBO (screen) */
  GkRenderModelFn    renderModelFn;
  GkRect             vrect;
  uint32_t           lightCount;
  uint32_t           lastLightIndex;
  int32_t            usage;
  GkSceneFlags       flags;
  int32_t            internalFormat;
  float              backingScale;
  float              fpsApprx;
} GkScene;
  
GK_INLINE
struct GkContext*
gkContextOf(GkScene * __restrict scene) {
  return scene->_priv.ctx;
}

GK_EXPORT
void
gkEnableShadows(GkScene * __restrict scene);

GK_EXPORT
void
gkDisableShadows(GkScene * __restrict scene);

GK_EXPORT
void
gkCacheTransformsFor(GkScene  * __restrict scene,
                     GkCamera * __restrict cam);

GK_EXPORT
void
gkRemoveTransformCacheFor(GkScene  * __restrict scene,
                          GkCamera * __restrict cam);

#ifdef __cplusplus
}
#endif
#endif /* gk_scene_h */
