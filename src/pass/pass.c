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

#include "../common.h"
#include "../../include/gk/model.h"
#include "../../include/gk/pass.h"
#include "../../include/gk/shader.h"
#include "../../include/gk/material.h"
#include "../../include/gk/program.h"

#include "../shader/cmn_material.h"
#include "../state/gpu.h"

#include <ds/rb.h>

GK_EXPORT
GkPass*
gkAllocPass(GkContext * __restrict ctx) {
  GkPass *pass;

  pass         = calloc(1, sizeof(*pass));
  pass->output = gkAllocOutput(ctx);

  return pass;
}

GK_EXPORT
GkPass*
gkGetOrCreatPass(GkScene    *scene,
                 GkLight    *light,
                 GkPrimInst *primInst,
                 GkMaterial *mat) {
  GkPass    *pass;
  GkPipeline *prog;

  if ((prog = gkGetPiplineForCmnMat(scene, light, primInst, mat))) {
    pass = calloc(1, sizeof(*pass));
    pass->prog = prog;
    return pass;
  }

  /* TODO: custom shaders */
  /*
   if (!prog) {
   }
   */

  return NULL;
}

GK_EXPORT
void
gkBindPass(GkScene * __restrict scene,
           GkPass  * __restrict pass) {
  GkContext     *ctx;
  GkColorOutput *color;

  ctx = gkContextOf(scene);

  gkDepthMask(ctx, GL_FALSE);
  gkEnableBlend(ctx);
  gkEnableCullFace(ctx); /* TODO: add option for this */

  /* apply options for all attachments*/
  if (pass->clear) {
    GkClearOp *clr;
    GLbitfield bit;

    clr = pass->clear;
    bit = 0;

    if (clr->color && clr->enabled) {
      float *c;
      c = *clr->color;
      glClearColor(c[0], c[1], c[2], c[3]);
    }

    if (pass->clear->clearColor)
      bit = GL_COLOR_BUFFER_BIT;

    if (pass->clear->clearDepth)
      bit |= GL_DEPTH_BUFFER_BIT;

    if (bit)
      glClear(bit);
  }

  if (pass->depthTest) {
    GkDepthTest *dt;

    dt = pass->depthTest;
    if (dt->enabled) {
      gkEnableDepthTest(ctx);
      gkDepthFunc(ctx, dt->func);
      gkDepthMask(ctx, dt->mask);
    } else {
      gkDisableBlend(ctx);
      gkDepthFunc(ctx, dt->func);
      gkDepthMask(ctx, dt->mask);
    }
  }

  if (pass->blend) {
    GkBlendOp *bl;

    bl = pass->blend;
    if (bl->enabled) {
      gkEnableBlend(ctx);
      gkBlendFunc(ctx, bl->src, bl->dst);
    } else {
      gkDisableBlend(ctx);
      gkBlendFunc(ctx, bl->src, bl->dst);
    }
  }

  /* apply per draw attachment opitons */
  color = pass->output->color;
  while (color) {
    if (color->clear)
      gkClearColor(color);

    if (color->blend) {
      GkBlendOp *bl;

      bl = color->blend;
      if (bl->enabled) {
        gkEnableBlendi(ctx, color->drawIndex);
        gkBlendFunci(ctx, color->drawIndex, bl->src, bl->dst);
      } else {
        gkDisableBlendi(ctx, color->drawIndex);
      }
    }

    color = color->next;
  }
}
