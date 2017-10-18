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

#include "common.h"
#include "shader.h"
#include "uniform.h"

#include <ds/rb.h>
#include <ds/hash.h>

#include <OpenGL/gl3.h>
#include <stdio.h>
#include <stdbool.h>

struct GkMaterial;
struct GkLight;
struct GkScene;

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
  HTable  *uniforms;

  struct GkMaterial *lastMaterial;
  struct GkLight    *lastLight;
  bool               updtLights;
  bool               updtMaterials;
} GkProgInfo;

void
gkProgramLogInfo(GLuint progId,
                 FILE * __restrict file);

bool
gkProgramIsValid(GLuint progId);

GkProgInfo*
gkNewProgram(GkShader *shaders);

GkProgInfo*
gkDefaultProgram(void);

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

GLint
gkCurrentProgram(void);

GK_EXPORT
void
gkUseProgram(struct GkScene *scene,
             GkProgInfo     *pinfo);

GkProgInfo*
gkGetOrCreatProg(char       *name,
                 GkProgInfo *(creatCb)(char *name, void *userData),
                 void       *userData);

#ifdef __cplusplus
}
#endif
#endif /* gk_program_h */
