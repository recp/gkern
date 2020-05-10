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
struct GkVertexInput;

typedef enum GkControllerType {
  GK_CONTROLLER_UNKNOWN = 0,
  GK_CONTROLLER_SKIN    = 1,
  GK_CONTROLLER_MORPH   = 2
} GkControllerType;

typedef enum GkMorphMethod {
  GK_MORPH_METHOD_NORMALIZED = 1,
  GK_MORPH_METHOD_RELATIVE   = 2,
  GK_MORPH_METHOD_ADDITIVE   = GK_MORPH_METHOD_RELATIVE
} GkMorphMethod;

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

typedef struct GkMorphTarget {
  struct GkMorphTarget *next;
  struct GkVertexInput *inputs;
  GkGpuBuffer          *bufs;
  float                 weight;
  uint32_t              nInputs;
  uint32_t              bufc;
} GkMorphTarget;

typedef enum GkMorphOrder {
  GK_IORD_P1P2N1N2 = 0,
  GK_IORD_P1N1P2N2 = 1
} GkMorphOrder;

typedef struct GkMorph {
  GkMorphTarget *targets;
  GkGpuBuffer   *buff;      /* must be interleaved */
  struct FList  *allInputs; /* shader ready inputs, NULL to reset */
  uint32_t       nTargets;
  GkMorphMethod  method;
  GkMorphOrder   order;
} GkMorph;

typedef struct GkInstanceMorph {
  GkModel *baseGeometry;
  GkMorph *morph;
  float   *overrideWeights; /* override default weights or NULL */
} GkInstanceMorph;

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
gkUniformTargetWeights(struct GkScene     * __restrict scene,
                       struct GkModelInst * __restrict modelInst,
                       float              * __restrict weights,
                       uint32_t                        nWeights);

GK_EXPORT
void
gkMakeInstanceSkin(struct GkScene          * __restrict scene,
                   struct GkNode           * __restrict node,
                   struct GkControllerInst * __restrict ctlrInst);

GK_EXPORT
void
gkDrawBones(struct GkScene * __restrict scene);

GK_EXPORT
void
gkPrepMorph(GkMorph * __restrict morph);

GK_EXPORT
void
gkAttachMorphTo(GkMorph     * __restrict morph,
                GkModelInst * __restrict modelInst);

GK_EXPORT
void
gkMakeInstanceMorph(struct GkScene         * __restrict scene,
                    struct GkNode          * __restrict node,
                    struct GkInstanceMorph * __restrict morphInst);

#endif /* gk_controller_h */
