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

#ifndef gk_options_h
#define gk_options_h
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

typedef enum GkOption {
  GK_OPT_LIGHT_DIR           = 0,  /* 0, 0, -1    */
  GK_OPT_LIGHT_UP            = 1   /* 0, 1,  0    */
} GkOption;

GK_EXPORT
void
gk_opt_set(GkOption option, uintptr_t value);

GK_EXPORT
uintptr_t
gk_opt(GkOption option);

#ifdef __cplusplus
}
#endif
#endif /* gk_options_h */

