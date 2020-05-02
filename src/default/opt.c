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
#include "../../include/gk/opt.h"

vec3 gk__light_dir = {0.0f, 0.0f, -1.0f};
vec3 gk__light_up  = {0.0f, 1.0f,  0.0f};

uintptr_t GK_OPTIONS[] =
{
  (uintptr_t)&gk__light_dir,
  (uintptr_t)&gk__light_up
};

GK_EXPORT
void
gk_opt_set(GkOption option, uintptr_t value) {
  GK_OPTIONS[option] = value;
}

GK_EXPORT
uintptr_t
gk_opt(GkOption option) {
  return GK_OPTIONS[option];
}
