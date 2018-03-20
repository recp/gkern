/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "def_effect.h"

GkColor gkdef_clr__blk = { 0.0f, 0.0f, 0.0f, 1.0f };
GkColor gkdef_clr__wht = { 1.0f, 1.0f, 1.0f, 1.0f };

GkColorOrTex gkdef_clr_blk = { .val    = &gkdef_clr__blk,
                               .method = GK_COLOR_COLOR };
GkColorOrTex gkdef_clr_wht = { .val    = &gkdef_clr__wht,
                               .method = GK_COLOR_COLOR };

GkPhong gkdef_phong = {
  .base = {
    .type       = GK_MATERIAL_PHONG,
    .subroutine = "phong"
  },
  .emission  = NULL,
  .ambient   = NULL,
  .diffuse   = &gkdef_clr_wht,
  .specular  = &gkdef_clr_wht,
  .shininess = 1.0f
};

GkBlinn gkdef_blinn = {
  .base = {
    .type       = GK_MATERIAL_BLINN,
    .subroutine = "blinn"
  },
  .emission  = NULL,
  .ambient   = NULL,
  .diffuse   = &gkdef_clr_wht,
  .specular  = &gkdef_clr_wht,
  .shininess = 1.0f,
};

GkLambert gkdef_lambert = {
  .base = {
    .type       = GK_MATERIAL_LAMBERT,
    .subroutine = "lambert"
  },
  .emission = NULL,
  .ambient  = NULL,
  .diffuse  = &gkdef_clr_wht,
};

GkConstant gkdef_constant = {
  .base = {
    .type       = GK_MATERIAL_CONSTANT,
    .subroutine = "constant"
  },
  .emission = NULL
};

GkMetalRough gkdef_metlrough = {
  .base = {
    .type       = GK_MATERIAL_METALROUGH,
    .subroutine = "mtlrough"
  },
  .albedo     = { 1.0f, 1.0f, 1.0f, 1.0f },
  .albedoTex  = NULL,
  .metalRoughTex = NULL,
  .metallic  = 1.0f,
  .roughness = 1.0f
};

GkPhong*
gk_def_material_phong() {
  return &gkdef_phong;
}

GkBlinn*
gk_def_material_blinn() {
  return &gkdef_blinn;
}

GkLambert*
gk_def_material_lambert() {
  return &gkdef_lambert;
}

GkConstant*
gk_def_material_constant() {
  return &gkdef_constant;
}

GkMetalRough*
gk_def_material_mtlrough() {
  return &gkdef_metlrough;
}

GkMaterial gkdef_material = {
  .technique         = &gkdef_lambert.base,
  .isvalid           = 1,
  .enabled           = 1,
  .reflective        = NULL,
  .transparent       = NULL,
  .indexOfRefraction = 1.0f
};

GkMaterial*
gk_def_material() {
  return &gkdef_material;
}
