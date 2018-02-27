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
#include "geom-types.h"
#include <ds/forward-list.h>
#include <stdbool.h>

struct GkScene;
struct GkContext;
struct GkPrimitive;
struct GkLight;

typedef struct GkClearOp {
  vec4    *color;
  float    depth;
  bool     clearColor:1;
  bool     clearDepth:1;
  bool     enabled;
} GkClearOp;

typedef struct GkBlendOp {
  bool     enabled;
  bool     separate;
  GLenum   src;
  GLenum   dst;
} GkBlendOp;

typedef struct GkDepthTest {
  bool        enabled;
  GLenum      func;
  GLboolean   mask;
} GkDepthTest;

/* GL_COLOR_ATTACHMENT[n] */
typedef struct GkColorOutput {
  GLuint                buffId;
  GLsizei               width;
  GLsizei               height;
  GLenum                attachment;
  GLenum                drawIndex;
  GkClearOp            *clear;
  GkBlendOp            *blend;
  struct GkColorOutput *next;
} GkColorOutput;

typedef struct GkOutput {
  GLuint         fbo;
  GLuint         depth;   /* GL_DEPTH_ATTACHMENT   */
  GLuint         stencil; /* GL_STENCIL_ATTACHMENT */
  GkColorOutput *color;   /* GL_COLOR_ATTACHMENT0  */
  GkClearOp     *clear;
  GkBlendOp     *blend;
  uint32_t       colorCount;
  bool           cleared;
} GkOutput;

typedef struct GkPass {
  GkProgram     *prog;
  FListItem     *states;
  GkOutput      *output;
  struct GkPass *inPasses;
  struct GkPass *outPass;
  struct GkPass *next;
  GkClearOp     *clear;
  GkBlendOp     *blend;
  GkDepthTest   *depthTest;
  bool           noLights;
  bool           noMaterials;
} GkPass;

GK_EXPORT
GkOutput*
gkDefaultRenderOut(void);

GK_EXPORT
GkOutput*
gkCurrentOutput(struct GkContext * __restrict ctx);

GK_EXPORT
GkPass*
gkGetOrCreatPass(struct GkScene     *scene,
                 struct GkLight     *light,
                 struct GkPrimitive *prim,
                 GkMaterial         *mat);

GK_EXPORT
void
gkBindPass(struct GkScene * __restrict scene,
           GkPass         * __restrict pass);

GK_EXPORT
GkPass*
gkAllocPass(void);

GK_EXPORT
GkOutput*
gkAllocOutput(void);

GK_EXPORT
void
gkBindOutput(struct GkScene *scene,
              GkOutput       *output);

GK_EXPORT
void
gkBindDefaultOutput(struct GkScene *scene);

GK_EXPORT
void
gkPassEnableDepth(struct GkScene *scene,
                  GkPass         *pass);

GK_EXPORT
void
gkPassEnableDepthTex(struct GkScene *scene,
                     GkPass         *pass,
                     GkSize          size);

GK_EXPORT
void
gkPassEnableDepthTexArray(struct GkScene *scene, GkPass *pass, GLsizei len);

GK_EXPORT
void
gkPassEnableDepthCubeTex(struct GkScene *scene, GkPass *pass, float size);

GK_EXPORT
GkColorOutput*
gkGetRenderTarget(GkPass *rt, int32_t index);

GK_EXPORT
void
gkBindRenderTargetToTexUnit(GkColorOutput *rt, int32_t texUnit);

GK_EXPORT
void
gkBindRenderTargetTo(struct GkScene *scene,
                        GkPass         *pass,
                        int32_t         targetIndex,
                        GkProgram      *prog,
                        int32_t         texUnit,
                        const char      *uniformName);

GK_EXPORT
void
gkBindDepthTexTo(struct GkScene *scene,
                 GkPass         *pass,
                 GkProgram      *prog,
                 int32_t         texUnit,
                 const char     *uniformName);

GK_EXPORT
void
gkBindDepthTexArrayTo(struct GkScene *scene,
                      GkPass         *pass,
                      GkProgram      *prog,
                      int32_t         texUnit,
                      const char     *uniformName);

GK_EXPORT
GLuint
gkAddRenderTarget(struct GkScene *scene,
                  GkPass         *pass,
                  GLenum          internalformat,
                  GLenum          format,
                  GLenum          type);

GK_EXPORT
GLuint
gkAddRenderTargetRB(struct GkScene *scene,
                    GkPass         *pass);

GK_EXPORT
GLuint
gkAddRenderTargetEx(struct GkScene *scene,
                    GkPass *pass,
                    GLenum  internalFormat,
                    GLenum  format,
                    GLsizei width,
                    GLsizei height,
                    GLenum  type);

GK_EXPORT
GLuint
gkAddRenderTargetRBEx(struct GkScene *scene,
                      GkPass *pass,
                      GLenum  internalFormat,
                      GLsizei width,
                      GLsizei height);

GK_EXPORT
void
gkClearColor(GkColorOutput *colorOutput);

GK_EXPORT
void
gkClearColorAt(GkOutput *output, int32_t buffIndex);

GK_EXPORT
void
gkClearColors(GkOutput *output);

#endif /* gk_pass_h */
