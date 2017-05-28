/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_src_matrix_h
#define gk_src_matrix_h

#include "../include/gk.h"

void
gkCalcViewMat(GkScene     * __restrict scene,
              GkTransform * __restrict tr);

void
gkCalcFinalMat(GkScene     * __restrict scene,
               GkTransform * __restrict tr);

#endif /* gk_src_matrix_h */
