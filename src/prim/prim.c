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

static
_gk_hide
void
gk_va_bindInputTo(GkVertexAttachment * __restrict va,
                  GkVertexInput      * __restrict inp,
                  int32_t                         startLoc) {
  GkVertexInputBind *last, *inpBind;
  GkGPUAccessor *acc;
  
  if (!(acc = inp->accessor))
    return;
  
  inpBind          = calloc(1, sizeof(*inpBind));
  inpBind->input   = inp;
  inpBind->enabled = true;

  if ((last = va->lastInput)) {
    last->next              = inpBind;
    inpBind->attribLocation = last->attribLocation + 1;
  } else {
    va->firstInput          = inpBind;
    inpBind->attribLocation = startLoc;
  }

  if (gkAccessorIsInteger(acc)) {
    glVertexAttribIPointer(inpBind->attribLocation,
                           acc->itemCount,
                           acc->itemType,
                           (GLsizei)acc->byteStride,
                           BUFFER_OFFSET(acc->byteOffset));
  } else {
    glVertexAttribPointer(inpBind->attribLocation,
                          acc->itemCount,
                          acc->itemType,
                          GL_FALSE,
                          (GLsizei)acc->byteStride,
                          BUFFER_OFFSET(acc->byteOffset));
  }
  
  glEnableVertexAttribArray(inpBind->attribLocation);
  va->lastInput = inpBind;
}

GK_EXPORT
void
gk_bindInputTo(GkPrimitive   * __restrict prim,
               GkVertexInput * __restrict inp) {
  gk_va_bindInputTo(&prim->vertex, inp, 0);
}

GK_EXPORT
void
gk_attachInputTo(GkPrimInst         * __restrict primInst,
                 GkVertexAttachment * __restrict va,
                 GkVertexInput      * __restrict inp) {
  GkVertexAttachment *va_last;
  int32_t             startLoc;

  if (!va->firstInput) {
    if ((va_last = primInst->vertexAttachments)) {
      while (va_last->next)
        va_last = va_last->next;
      
      va_last->next = va;
    } else {
      va_last = &primInst->prim->vertex;
    }
    
    startLoc = va_last->lastInput->attribLocation + 1;
  } else {
    startLoc = 0;
  }
 
  gk_va_bindInputTo(va, inp, startLoc);
}
