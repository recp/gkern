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
#include "../../include/gk/gk.h"
#include "../../include/gk/material.h"
#include <string.h>

#include "../default/def_effect.h"

GkTechnique*
gkMaterialNewPhong() {
  GkTechnique *phong;
  phong = malloc(sizeof(*phong));
  memcpy(phong,
         gk_def_material_phong(),
         sizeof(*phong));
  return phong;
}

GkTechnique*
gkMaterialNewBlinn() {
  GkTechnique *blinn;
  blinn = malloc(sizeof(*blinn));
  memcpy(blinn,
         gk_def_material_blinn(),
         sizeof(*blinn));
  return blinn;
}

GkTechnique*
gkMaterialNewLambert() {
  GkTechnique *lambert;
  lambert = malloc(sizeof(*lambert));
  memcpy(lambert,
         gk_def_material_lambert(),
         sizeof(*lambert));
  return lambert;
}

GkTechnique*
gkMaterialNewConstant() {
  GkTechnique *constant;
  constant = malloc(sizeof(*constant));
  memcpy(constant,
         gk_def_material_constant(),
         sizeof(*constant));
  return constant;
}

GkMetalRough*
gkMaterialNewMetalRough() {
  GkMetalRough *mtlrough;
  mtlrough = malloc(sizeof(*mtlrough));
  memcpy(mtlrough,
         gk_def_material_mtlrough(),
         sizeof(*mtlrough));
  return mtlrough;
}

GkSpecGloss*
gkMaterialNewSpecGloss() {
  GkSpecGloss *specGloss;
  specGloss = malloc(sizeof(*specGloss));
  memcpy(specGloss,
         gk_def_material_specgloss(),
         sizeof(*specGloss));
  return specGloss;
}
