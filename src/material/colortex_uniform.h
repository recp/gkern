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

#ifndef gk_colortex_uniform_h
#define gk_colortex_uniform_h

#include "../../include/gk/gk.h"

void
gkUniformColor(GkColor   * __restrict color,
               char      * __restrict name,
               GkPipeline * __restrict prog);

void
gkUniformTex(GkContext  * __restrict ctx,
             GkMaterial * __restrict mat,
             GkTexture  * __restrict tex,
             char       * __restrict name,
             GkPipeline  * __restrict prog);

void
gkUniformColorDesc(GkContext    * __restrict ctx,
                    GkMaterial  * __restrict mat,
                    GkColorDesc * __restrict crtx,
                    char        * __restrict name,
                    GkPipeline   * __restrict prog);

void
gkUniformColorBuff(GkColor   * __restrict color,
                   char      * __restrict buf,
                   char      * __restrict name,
                   GkPipeline * __restrict prog);

void
gkUniformColorDescBuff(GkContext   * __restrict ctx,
                       GkMaterial  * __restrict mat,
                       GkColorDesc * __restrict crtx,
                       char        * __restrict buf,
                       char        * __restrict name,
                       GkPipeline   * __restrict prog);

#endif /* gk_colortex_uniform_h */
