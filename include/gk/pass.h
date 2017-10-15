/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_pass_h
#define gk_pass_h

#include "common.h"
#include "program.h"
#include "material.h"

#include <ds/forward-list.h>

struct GkScene;

/* GL_COLOR_ATTACHMENT[n] */
typedef struct GkPassOutColor {
  GLuint                 buffId;
  GLsizei                width;
  GLsizei                height;
  GLenum                 attachment;
  struct GkPassOutColor *next;
} GkPassOutColor;

typedef struct GkPassOut {
  GLuint          fbo;
  GLuint          depth;   /* GL_DEPTH_ATTACHMENT   */
  GLuint          stencil; /* GL_STENCIL_ATTACHMENT */
  GkPassOutColor *color;   /* GL_COLOR_ATTACHMENT0  */
  uint32_t        colorCount;
} GkPassOut;

typedef struct GkPass {
  GkProgInfo    *pinfo;
  FListItem     *states;
  GkPassOut     *output;
  struct GkPass *inPasses;
  struct GkPass *outPass;
  struct GkPass *next;
} GkPass;

GK_EXPORT
GkPass*
gkGetOrCreatPass(GkMaterial *mat);

GK_EXPORT
GkPassOut*
gkAllocPassOut(void);

GK_EXPORT
void
gkBindPassOut(GkPassOut *pout);

GK_EXPORT
void
gkBindDefaultPassOut(void);

GK_EXPORT
GLuint
gkAddRenderTarget(struct GkScene *scene,
                  GkPass         *pass,
                  GLenum          format);

GK_EXPORT
GLuint
gkAddRenderTargetRB(struct GkScene *scene,
                    GkPass         *pass);

GK_EXPORT
GLuint
gkAddRenderTargetEx(GkPass *pass,
                    GLenum  internalFormat,
                    GLenum  format,
                    GLsizei width,
                    GLsizei height,
                    GLenum  type);

GK_EXPORT
GLuint
gkAddRenderTargetRBEx(GkPass *pass,
                      GLenum  internalFormat,
                      GLsizei width,
                      GLsizei height);
#endif /* gk_pass_h */
