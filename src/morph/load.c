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

#include "../../include/gk/gk.h"
#include "../../include/gk/clear.h"
#include "../../include/gk/pass.h"
#include "../../include/gk/gpu_state.h"
#include "../../include/gk/prims/builtin-prim.h"
#include "../../include/gk/vertex.h"
#include "../types/impl_scene.h"
#include "../program/uniform_cache.h"
#include "../shader/builtin_shader.h"

#include <string.h>

GK_EXPORT
void
gkPrepMorph(GkMorph * __restrict morph) {
  GkMorphTarget  *target;
  GkVertexInput  *vi;
  GkVertexInput **targetInputs;
  int32_t        *targetInputCounts, maxTargetCount, i, cont;

  if (morph->allInputs)
    return;

  i                 = cont = 0;
  maxTargetCount    = 4;
  target            = morph->targets;
  
  targetInputs      = alloca(sizeof(void*) * maxTargetCount);
  targetInputCounts = alloca(sizeof(uint32_t) * maxTargetCount);
  
  memset(targetInputs, 0, sizeof(void*) * maxTargetCount);
  memset(targetInputCounts, 0, sizeof(uint32_t) * maxTargetCount);

  while (target) {
    if (maxTargetCount-- < 1)
      break;

    /* there is no attachment */
    if (!(vi = target->inputs))
      continue;
    
    targetInputs[i]        = vi;
    targetInputCounts[i++] = target->nInputs;
    cont++;

    target = target->next;
  } /* while (target) */
  
  morph->allInputs = flist_new(NULL);
  
  if (morph->order == GK_IORD_P1P2N1N2) {
    while (cont > 0) {
      for (i = 0; i < maxTargetCount; i++) {
        if ((vi = targetInputs[i])) {
          flist_append(morph->allInputs, vi);
          targetInputs[i] = vi->next;

          if (--targetInputCounts[i] < 1)
            cont--;
        }
      }
    }
  } else {
    for (i = 0; i < maxTargetCount; i++) {
      if ((vi = targetInputs[i])) {
        do {
          flist_append(morph->allInputs, vi);
        } while ((vi = vi->next));
      } /* if */
    } /* for */
  }
}

GK_EXPORT
void
gkAttachMorphTo(GkMorph     * __restrict morph,
                GkModelInst * __restrict modelInst) {
  GkPrimInst         *prim;
  GkGpuBuffer        *gbuff;
  GkVertexAttachment *va, *va_last;
  FListItem          *inputItem;
  size_t              i;
  uint32_t            nPrims;
  
  if (!morph->allInputs)
    gkPrepMorph(morph);

  if (!morph->allInputs || !morph->allInputs->first)
    return;

  /* currently allow only 4 target */
  nPrims = modelInst->primc;
  gbuff  = morph->buff;

  /* for each primitive */
  for (i = 0; i < nPrims; i++) {
    prim = &modelInst->prims[i];
    if (prim->hasMorph)
      continue;

    gkBindPrimInst(prim);
    
    /* bind interleaved morph buffer */
    glBindBuffer(gbuff->target, gbuff->vbo);
    
    /* there is no attachment */
    if (!(inputItem = morph->allInputs->first))
      continue;

    prim->hasMorph = true;
    va             = calloc(1, sizeof(*va));
    va->semantic   = GK_VERT_ATTACH_MORPH;

    do {
      gk_attachInputTo(prim, va, inputItem->data);
    } while ((inputItem = inputItem->next));
    
    if ((va_last = prim->vertexAttachments)) {
      while (va_last->next)
        va_last = va_last->next;
      
      va_last->next = va;
    } else {
      prim->vertexAttachments = va;
    }
  }

  modelInst->morpher = morph;
}

GK_EXPORT
void
gkUniformTargetWeights(GkScene     * __restrict scene,
                       GkModelInst * __restrict modelInst,
                       float       * __restrict weights,
                       uint32_t                 nWeights) {
  glBindBuffer(GL_UNIFORM_BUFFER, modelInst->uboTargetWeights->vbo);
  glBufferSubData(GL_UNIFORM_BUFFER,
                  0,
                  sizeof(*weights) * nWeights,
                  weights);
  glBindBuffer(modelInst->uboTargetWeights->target, 0);
}

GK_EXPORT
void
gkMakeInstanceMorph(struct GkScene         * __restrict scene,
                    struct GkNode          * __restrict node,
                    struct GkInstanceMorph * __restrict morphInst) {
  GkSceneImpl *sceneImpl;

  sceneImpl     = (GkSceneImpl *)scene;
  node->morpher = morphInst;

  flist_sp_insert(&sceneImpl->instMorphs, node);
}
