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

GkColorDesc gkdef_clr_blk = { .val    = &gkdef_clr__blk,
                              .method = GK_COLOR_COLOR };
GkColorDesc gkdef_clr_wht = { .val    = &gkdef_clr__wht,
                              .method = GK_COLOR_COLOR };

GkTechnique gkdef_phong = {
  .type      = GK_MATERIAL_PHONG,
  .ambient   = NULL,
  .diffuse   = &gkdef_clr_wht,
  .specular  = &gkdef_clr_wht,
  .shininess = 1.0f
};

GkTechnique gkdef_blinn = {
  .type      = GK_MATERIAL_BLINN,
  .ambient   = NULL,
  .diffuse   = &gkdef_clr_wht,
  .specular  = &gkdef_clr_wht,
  .shininess = 1.0f,
};

GkTechnique gkdef_lambert = {
  .type    = GK_MATERIAL_LAMBERT,
  .ambient = NULL,
  .diffuse = &gkdef_clr_wht,
};

GkTechnique gkdef_constant = {
  .type = GK_MATERIAL_CONSTANT
};

GkMetalRough gkdef_metlrough = {
  .base = {
    .type = GK_MATERIAL_METALROUGH
  },
  .albedo     = { 1.0f, 1.0f, 1.0f, 1.0f },
  .albedoMap  = NULL,
  .metalRoughMap = NULL,
  .metallic  = 1.0f,
  .roughness = 1.0f
};

GkSpecGloss gkdef_specgloss = {
  .base = {
    .type = GK_MATERIAL_SPECGLOSS
  },
  .diffuse      = { 1.0f, 1.0f, 1.0f, 1.0f },
  .specular     = { 1.0f, 1.0f, 1.0f, 1.0f },
  .diffuseMap   = NULL,
  .specGlossMap = NULL,
  .gloss        = 1.0f
};

GkTechnique*
gk_def_material_phong() {
  return &gkdef_phong;
}

GkTechnique*
gk_def_material_blinn() {
  return &gkdef_blinn;
}

GkTechnique*
gk_def_material_lambert() {
  return &gkdef_lambert;
}

GkTechnique*
gk_def_material_constant() {
  return &gkdef_constant;
}

GkMetalRough*
gk_def_material_mtlrough() {
  return &gkdef_metlrough;
}

GkSpecGloss*
gk_def_material_specgloss() {
  return &gkdef_specgloss;
}

GkMaterial gkdef_material = {
  .technique         = &gkdef_lambert,
  .isvalid           = 1,
  .enabled           = 1,
  .indexOfRefraction = 1.0f
};

GkMaterial*
gk_def_material() {
  return &gkdef_material;
}
