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

GK_EXPORT
void
gkAttachMorphTo(GkMorph     * __restrict morph,
                GkModelInst * __restrict modelInst) {
  GkMorphTarget      *target;
  GkPrimInst         *prim;
  GkGpuBuffer        *gbuff;
  GkVertexAttachment *va;
  GkVertexInput      *vi;
  RBTree             *inpMap;
  size_t              i, j;
  uint32_t            nPrims, maxTargetCount;
  bool                vaIsUsed;
  
  /* currently allow only 4 target */
  maxTargetCount = 4;
  target         = morph->targets;
  nPrims         = modelInst->primc;
  gbuff          = morph->buff;
  va             = calloc(1, sizeof(*va));
  inpMap         = rb_newtree(NULL, gkVertexInputCmp, NULL);
  va->semantic   = GK_VERT_ATTACH_MORPH;
  vaIsUsed       = false;

  /* for each primitive */
  for (i = j = 0; i < nPrims; i++) {
    while (target) {
      if (j++ >= maxTargetCount)
        break;

      /* there is no attachment */
      if (!(vi = target->inputs))
        continue;

      prim = &modelInst->prims[i];
      gkBindPrimInst(prim);

      /* bind interleaved morph buffer */
      glBindBuffer(gbuff->target, gbuff->vbo);

      do {
        if ((rb_find(inpMap, vi)))
          continue;
        
        gk_attachInputTo(prim, va, vi);
        vaIsUsed = true;
      } while ((vi = vi->next));

      target = target->next;
    } /* while (target) */
  }
  
  if (!vaIsUsed)
    free(va);

  modelInst->morpher = morph;
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
