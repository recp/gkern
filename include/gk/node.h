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

#ifndef gk_node_h
#define gk_node_h
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include "light.h"
#include "bbox.h"
#include "model.h"
#include "camera.h"
#include "transform.h"
#include "controller.h"

#include <stdlib.h>
#include <cglm/cglm.h>

struct GkScene;
struct GkAnimatable;
struct GkControllerInst;

typedef enum GkNodeFlags {
  GK_NODEF_NONE           = 0,
  GK_NODEF_NODE           = 1,
  GK_NODEF_HAVE_TRANSFORM = 2,
  GK_NODEF_JOINT          = 3
} GkNodeFlags;

typedef struct GkNode {
  struct GkNode        *parent;
  struct GkNode        *next;
  struct GkNode        *chld;

  struct GkAnimatable  *anim;
  GkTransform          *trans;

  GkModelInst          *model;
  GkLight              *light;
  GkCamera             *camera;
  GkControllerInst     *controller;
  GkInstanceMorph      *morpher;

  GkNodeFlags           flags;
} GkNode;

GkNode*
gkAllocNode(struct GkScene * __restrict scene);

void
gkMakeNodeTransform(struct GkScene * __restrict scene,
                    GkNode         * __restrict node);

GK_EXPORT
void
gkApplyTransform(struct GkScene * __restrict scene,
                 GkNode         * __restrict node);

GK_EXPORT
void
gkApplyView(struct GkScene * __restrict scene,
            GkNode         * __restrict node);

#ifdef __cplusplus
}
#endif
#endif /* gk_node_h */
