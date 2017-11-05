/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef src_gpu_state_impl_h
#define src_gpu_state_impl_h

#include "../../include/gk/gk.h"
#include "../../include/gk/gpu_state.h"

#include "common.h"

#define GK_GPU_APPLY_ST_FN(NAME)                                               \
  _gk_hide void NAME(GkContext * __restrict, GkStateBase * __restrict);

GK_GPU_APPLY_ST_FN(gkApplyDepthState);
GK_GPU_APPLY_ST_FN(gkApplyBlendState);
GK_GPU_APPLY_ST_FN(gkApplyTexState);
GK_GPU_APPLY_ST_FN(gkApplyOutputState);

#endif /* src_gpu_state_impl_h */
