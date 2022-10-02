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

#include "../../common.h"
#include "../../../include/gk/gk.h"

#include "rn_light.h"
#include "rn_pass.h"
#include "rn_transp.h"

bool
gkPrimIsTransparent(GkScene     * __restrict scene,
                    GkGeometryInst * __restrict modelInst,
                    GkPrimInst  * __restrict primInst) {
  return gkIsTransparent(scene, primInst->activeMaterial);
}

bool
gkIsTransparent(GkScene    * __restrict scene,
                GkMaterial * __restrict mat) {
  GkColor     *color;
  GkTechnique *tech;
  float        opacity;

  if (!(scene->flags & GK_SCENEF_TRANSP)
      || !(tech = mat->technique)
      || !tech->transparent)
    return false;

  color = NULL;
  if (tech->transparent->color
      && tech->transparent->color->method == GK_COLOR_COLOR)
    color = tech->transparent->color->val;

  switch (tech->transparent->opaque) {
    case GK_OPAQUE_A_ONE: {
      opacity = tech->transparent->amount;

      if (color)
        return (color->rgba.A * opacity) < 1.0f;

      return true;
      /* return opacity < 1.0f; */
    }
    case GK_OPAQUE_A_ZERO: {
      opacity = 1.0f - tech->transparent->amount;

      if (color)
        return (color->rgba.A * opacity) > 0.0f;

      return true;
      /* return opacity < 1.0f; */
    }
    case GK_OPAQUE_RGB_ONE: {
      opacity = tech->transparent->amount;

      if (color)
        return (glm_luminance(color->vec) * opacity) < 1.0f;

      return true;
      /* return opacity < 1.0f; */
    }
    case GK_OPAQUE_RGB_ZERO: {
      opacity = 1.0f - tech->transparent->amount;

      if (color)
        return (glm_luminance(color->vec) * opacity) > 0.0f;

      return true;
      /* return opacity < 1.0f; */
    }
    case GK_OPAQUE_MASK: {
      return false;
    }

    default: return true;
  }
}
