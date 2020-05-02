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

#ifndef gk_src_common_h
#define gk_src_common_h

#define GK_INTERN

#ifdef _MSC_VER
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>

#  define strncasecmp _strnicmp
#  define strcasecmp  _stricmp
#endif

#ifdef __GNUC__
#  define GK_DESTRUCTOR __attribute__((destructor))
#  define GK_CONSTRUCTOR __attribute__((constructor))
#else
#  define GK_DESTRUCTOR
#  define GK_CONSTRUCTOR
#endif

#define GK__UNUSED(X) (void)X

#include "types/impl_transform.h"
#include "types/impl_scene.h"
#include "error/gpu/err.h"

#endif /* gk_src_common_h */
