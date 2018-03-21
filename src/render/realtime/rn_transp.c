/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../common.h"
#include "../../../include/gk/gk.h"

#include "rn_light.h"
#include "rn_pass.h"
#include "rn_transp.h"

bool
gkPrimIsTransparent(GkScene     * __restrict scene,
                    GkModelInst * __restrict modelInst,
                    GkPrimInst  * __restrict primInst) {
  return gkIsTransparent(scene, primInst->activeMaterial);
}

bool
gkIsTransparent(GkScene    * __restrict scene,
                GkMaterial * __restrict mat) {
  GkColor     *color;
  GkTechnique *tech;

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
      float opacity;
      opacity =  tech->transparent->amount;

      if (color)
        return (color->rgba.A * opacity) < 1.0f;

      return opacity < 1.0f;
    }
    case GK_OPAQUE_A_ZERO: {
      float opacity;
      opacity = 1.0f - tech->transparent->amount;

      if (color)
        return (glm_luminance(color->vec) * opacity) < 1.0f;

      return opacity < 1.0f;
    }
    case GK_OPAQUE_RGB_ONE: {
      float opacity;
      opacity = tech->transparent->amount;

      if (color)
        return (glm_luminance(color->vec) * opacity) < 1.0f;

      return opacity < 1.0f;
    }
    case GK_OPAQUE_RGB_ZERO: {
      float opacity;
      opacity = 1.0f - tech->transparent->amount;

      if (color)
        return (glm_luminance(color->vec) * opacity) < 1.0f;

      return opacity < 1.0f;
    }

    default: return true;
  }
}
