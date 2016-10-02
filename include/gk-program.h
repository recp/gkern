/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_program_h
#define gk_program_h
#ifdef __cplusplus
extern "C" {
#endif

#include <OpenGL/gl3.h>
#include <stdio.h>
#include <stdbool.h>

void
gkProgramLogInfo(GLuint progId,
                 FILE * __restrict file);

bool
gkProgramIsValid(GLuint progId);

#ifdef __cplusplus
}
#endif
#endif /* gk_program_h */
