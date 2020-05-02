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

#ifndef uniform_h
#define uniform_h

#include "common.h"

struct GkPipeline;

GK_EXPORT
void
gkUniform1i(struct GkPipeline * __restrict prog,
            const char       * __restrict name,
            GLint                         val);

GK_EXPORT
void
gkUniform1ui(struct GkPipeline * __restrict prog,
             const char       * __restrict name,
             GLuint                        val);

GK_EXPORT
void
gkUniform1f(struct GkPipeline  * __restrict prog,
            const char        * __restrict name,
            float                          val);

GK_EXPORT
void
gkUniform2f(struct GkPipeline  * __restrict prog,
            const char        * __restrict name,
            float                          val[2]);

#endif /* uniform_h */
