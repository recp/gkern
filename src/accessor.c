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

#include "common.h"
#include "../include/gk/accessor.h"

GK_EXPORT
bool
gkAccessorIsInteger(GkGPUAccessor * __restrict acc) {
  switch (acc->itemType) {
    case GL_INT:
    case GL_UNSIGNED_INT:
    case GL_SHORT:
    case GL_UNSIGNED_SHORT:
    case GL_BYTE:
    case GL_UNSIGNED_BYTE: return true;
    default:
      return false;
  }
}
