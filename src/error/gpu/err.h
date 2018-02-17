/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef src_err_gpu_err_h
#define src_err_gpu_err_h

#include "../../common.h"

GK_EXPORT
void
gkGPUAssertOnError(void);

#ifdef DEBUG
#  define GK_DEBUG_GPU_ASSERT_ONERROR() gkGPUAssertOnError()
#endif

#endif /* src_err_gpu_err_h */
