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
  GLenum   src;
  GLenum   dst;
} GkBlendOp;

/* GL_COLOR_ATTACHMENT[n] */
typedef struct GkPassOutColor {
  GLuint                 buffId;
  GLsizei                width;
  GLsizei                height;
  GLenum                 attachment;
  GkClearOp             *clear;
  GkBlendOp             *blend;
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
  GkProgram     *prog;
  FListItem     *states;
  GkPassOut     *output;
  struct GkPass *inPasses;
  struct GkPass *outPass;
  struct GkPass *next;
  bool           noLights;
  bool           noMaterials;
} GkPass;

GK_EXPORT
GkPassOut*
gkDefaultRenderOut(void);

GK_EXPORT
GkPassOut*
gkCurrentOutput(struct GkContext * __restrict ctx);

GK_EXPORT
GkPass*
gkGetOrCreatPass(struct GkScene     *scene,
                 struct GkLight     *light,
                 struct GkPrimitive *prim,
                 GkMaterial         *mat);

GK_EXPORT
GkPass*
gkAllocPass(void);

GK_EXPORT
GkPassOut*
gkAllocPassOut(void);

GK_EXPORT
void
gkBindPassOut(struct GkScene *scene,
              GkPassOut      *pout);

GK_EXPORT
void
gkBindDefaultPassOut(struct GkScene *scene);

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
GkPassOutColor*
gkGetRenderTarget(GkPass *rt, int32_t index);

GK_EXPORT
void
gkBindRenderTargetToTexUnit(GkPassOutColor *rt, int32_t texUnit);

GK_EXPORT
void
gkBindRenderTargetToTex(struct GkScene *scene,
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
gkClearColor(GkPassOutColor *poutColor);

#endif /* gk_pass_h */
