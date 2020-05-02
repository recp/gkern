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
  mat4           *invBindPoses;    /* inv joint matrices           */
  struct GkNode **joints;
  GkBoneWeights **weights;         /* per primitive Client buffers */
  GkGpuBuffer   **gbuffs;          /* per primitive GPU buffers    */
  mat4            bindShapeMatrix;
  size_t          nJoints;
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
gkAttachSkinTo(GkSkin      * __restrict skin,
               GkModelInst * __restrict modelInst);

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
