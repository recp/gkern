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

#include "../../include/gk/gk.h"
#include "../../include/gk/vertex.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

GK_EXPORT
void
gk_bindInputTo(GkPrimitive   * __restrict prim,
               GkVertexInput * __restrict inp) {
  GkGPUAccessor *acc;
  
  if (!(acc = inp->accessor))
    return;
  
  if (gkAccessorIsInteger(acc)) {
    glVertexAttribIPointer(prim->lastInputIndex,
                           acc->itemCount,
                           acc->itemType,
                           (GLsizei)acc->byteStride,
                           BUFFER_OFFSET(acc->byteOffset));
  } else {
    glVertexAttribPointer(prim->lastInputIndex,
                          acc->itemCount,
                          acc->itemType,
                          GL_FALSE,
                          (GLsizei)acc->byteStride,
                          BUFFER_OFFSET(acc->byteOffset));
  }
  
  glEnableVertexAttribArray(prim->lastInputIndex);
  prim->lastInputIndex++;
}

GK_EXPORT
void
gk_attachInputTo(GkPrimInst         * __restrict primInst,
                 GkVertexAttachment * __restrict va,
                 GkVertexInput      * __restrict inp) {
  GkGPUAccessor *acc;
  
  if (!(acc = inp->accessor))
    return;
  
  if (gkAccessorIsInteger(acc)) {
    glVertexAttribIPointer(primInst->lastInputIndexAll,
                           acc->itemCount,
                           acc->itemType,
                           (GLsizei)acc->byteStride,
                           BUFFER_OFFSET(acc->byteOffset));
  } else {
    glVertexAttribPointer(primInst->lastInputIndexAll,
                          acc->itemCount,
                          acc->itemType,
                          GL_FALSE,
                          (GLsizei)acc->byteStride,
                          BUFFER_OFFSET(acc->byteOffset));
  }
  
  glEnableVertexAttribArray(primInst->lastInputIndexAll);
  primInst->lastInputIndexAll++;
}
