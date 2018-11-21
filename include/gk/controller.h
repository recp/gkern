/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_controller_h
#define gk_controller_h

#include "common.h"
#include "value.h"

struct GkAnimation;
struct GkScene;
struct GkNode;
struct GkChannel;
struct GkPipeline;

typedef enum GkControllerType {
  GK_CONTROLLER_UNKNOWN = 0,
  GK_CONTROLLER_SKIN    = 1,
  GK_CONTROLLER_MORPH   = 2
} GkControllerType;

typedef struct GkController {
  struct GkController *next;
  void                *source; /* source geometry or controller */
  GkControllerType     type;
} GkController;

typedef struct GkBoneWeight {
  uint32_t joint;
  float    weight;
} GkBoneWeight;

typedef struct GkBoneWeights {
  uint16_t     *pJointsCount;
  size_t       *pWeightsIndex;
  GkBoneWeight *weights;
  size_t        nWeights;
  size_t        nVertex;
} GkBoneWeights;

typedef struct GkSkin {
  GkController    base;
  mat4           *invBindMatrices;
  mat4           *jointMatrices; /* cached matrices              */
  struct GkNode **joints;
  GkBoneWeights **weights;       /* per primitive Client buffers */
  GkGpuBuffer   **gbuffs;        /* per primitive GPU buffers    */
  mat4            bindShapeMatrix;
  uint32_t        nJoints;
  uint32_t        nPrims;
} GkSkin;

typedef struct GkMorph {
  void *targets;
} GkMorph;

typedef struct GkControllerInst {
  struct GkControllerInst *next;
  GkController            *ctlr;
  struct GkNode          **joints;
} GkControllerInst;

GK_EXPORT
void
gkAttachSkin(GkSkin * __restrict skin);

GK_EXPORT
void
gkAttachSkinTo(GkSkin * __restrict skin, GkModelInst * __restrict modelInst);

GK_EXPORT
void
gkUniformJoints(struct GkScene     * __restrict scene,
                struct GkModelInst * __restrict modelInst);

GK_EXPORT
void
gkMakeInstanceSkin(struct GkScene          * __restrict scene,
                   struct GkNode           * __restrict node,
                   struct GkControllerInst * __restrict ctlrInst);

GK_EXPORT
void
gkDrawBones(struct GkScene * __restrict scene);

#endif /* gk_controller_h */
