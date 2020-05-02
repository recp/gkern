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

#ifndef gk_def_effect_h
#define gk_def_effect_h

#include "../../include/gk/gk.h"
#include "../../include/gk/material.h"

GkMaterial*
gk_def_material(void);

GkTechnique*
gk_def_material_phong(void);

GkTechnique*
gk_def_material_blinn(void);

GkTechnique*
gk_def_material_lambert(void);

GkTechnique*
gk_def_material_constant(void);

GkMetalRough*
gk_def_material_mtlrough(void);

GkSpecGloss*
gk_def_material_specgloss(void);

#endif /* gk_def_effect_h */
