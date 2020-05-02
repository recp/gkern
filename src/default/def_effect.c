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
#include "def_effect.h"

GkColor gkdef_clr__blk = { 0.0f, 0.0f, 0.0f, 1.0f };
GkColor gkdef_clr__wht = { 1.0f, 1.0f, 1.0f, 1.0f };

GkColorDesc gkdef_clr_blk = { .val    = &gkdef_clr__blk,
                              .method = GK_COLOR_COLOR };
GkColorDesc gkdef_clr_wht = { .val    = &gkdef_clr__wht,
                              .method = GK_COLOR_COLOR };

GkTechnique gkdef_phong = {
  .pass      = NULL,
  .type      = GK_MATERIAL_PHONG,
  .ambient   = NULL,
  .diffuse   = &gkdef_clr_wht,
  .specular  = &gkdef_clr_wht,
  .shininess = 1.0f
};

GkTechnique gkdef_blinn = {
  .pass      = NULL,
  .type      = GK_MATERIAL_BLINN,
  .ambient   = NULL,
  .diffuse   = &gkdef_clr_wht,
  .specular  = &gkdef_clr_wht,
  .shininess = 1.0f,
};

GkTechnique gkdef_lambert = {
  .pass    = NULL,
  .type    = GK_MATERIAL_LAMBERT,
  .ambient = NULL,
  .diffuse = &gkdef_clr_wht,
};

GkTechnique gkdef_constant = {
  .pass = NULL,
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
