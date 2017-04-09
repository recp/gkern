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

#include "gk-common.h"
#include "gk-rb.h"
#include <OpenGL/gl3.h>
#include <stdio.h>
#include <stdbool.h>

/* cache program infos, some nodes may use different program and shaders */
typedef struct GkProgInfo {
  uint32_t refc;    /* reference count                                       */
  GLint    prog;    /* program                                               */
  GLint    mvpi;    /* matrix loc                                            */
  GLint    mvi;     /* matrix loc                                            */
  GLint    nmi;     /* normal matrix loc                                     */
  GLint    nmui;    /* use normal matrix loc                                 */
  uint32_t attribc; /* attrib count                                          */
  uint32_t attribl; /* last used attrib index, don't edit manually!          */
  RBTree  *attribs; /* attribs                                               */
} GkProgInfo;

void
gkProgramLogInfo(GLuint progId,
                 FILE * __restrict file);

bool
gkProgramIsValid(GLuint progId);

GkProgInfo*
gkNewProgram(GLuint vertShader,
             GLuint fragShader,
             GLint  modelViewProjection,
             GLint  modelView,
             GLint  normalMatrix,
             GLint  useNormalMatrix);

GkProgInfo*
gkDefaultProgram();

GLint
gk_progAttribIndex(GkProgInfo * __restrict prog,
                   const char * __restrict name);

void
gk_progSetAttribs(GkProgInfo * __restrict prog,
                  size_t      count,
                  const char *names[]);

GLint
gk_progAddAttrib(GkProgInfo * __restrict prog,
                 const char * __restrict name);

void
gk_progRemoveAttrib(GkProgInfo * __restrict prog,
                    const char * __restrict name,
                    bool                    shrink);

#ifdef __cplusplus
}
#endif
#endif /* gk_program_h */
