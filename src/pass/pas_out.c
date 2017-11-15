/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../include/gk/gk.h"
#include "../../include/gk/pass.h"
#include "../../include/gk/scene.h"
#include "../gpu_state/common.h"

GK_EXPORT
GkPassOut*
gkAllocPassOut(void) {
  GkPassOut *pout;

  pout = calloc(sizeof(*pout), 1);
  glGenFramebuffers(1, &pout->fbo);

  return pout;
}

GK_EXPORT
void
gkBindPassOut(GkScene   *scene,
              GkPassOut *pout) {
  GkRenderOutState *state;
  GkContext        *ctx;

  ctx   = scene->_priv.ctx;
  state = gkGetOrCreatState(ctx, GK_GPUSTATE_RENDER_OUT);
  if (ctx->currState->outputState.renderOutput == pout)
    return;

  state->renderOutput = pout;

  gkStateMakeCurrent(ctx, &state->base);
  glBindFramebuffer(GL_FRAMEBUFFER, pout->fbo);
}

GK_EXPORT
void
gkBindDefaultPassOut(GkScene *scene) {
  GkRenderOutState *state;
  GkContext        *ctx;
  
  ctx   = scene->_priv.ctx;
  state = gkGetOrCreatState(ctx, GK_GPUSTATE_RENDER_OUT);
  if (ctx->currState->outputState.renderOutput == NULL)
    return;
  
  state->renderOutput = NULL;
  gkStateMakeCurrent(ctx, &state->base);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GK_EXPORT
void
gkPassEnableDepth(GkScene *scene,
                  GkPass  *pass) {
  GkPassOut *pout;

  if (!(pout = pass->output))
    pass->output = pout = gkAllocPassOut();

  if (pout->depth != 0)
    return;

  glGenRenderbuffers(1, &pout->depth);
  glBindRenderbuffer(GL_RENDERBUFFER, pout->depth);

  glRenderbufferStorage(GL_RENDERBUFFER,
                        GL_DEPTH_COMPONENT,
                        scene->vrect.size.w * scene->backingScale,
                        scene->vrect.size.h * scene->backingScale);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                            GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER,
                            pout->depth);
}

GK_EXPORT
void
gkPassEnableDepthTex(GkScene *scene,
                     GkPass  *pass) {
  GkPassOut *pout;
  
  if (!(pout = pass->output))
    pass->output = pout = gkAllocPassOut();
  
  if (pout->depth != 0)
    return;
  
  gkBindPassOut(scene, pout);
  
  glGenTextures(1, &pout->depth);
  glBindTexture(GL_TEXTURE_2D, pout->depth);
  
  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_DEPTH_COMPONENT,
               scene->vrect.size.w * scene->backingScale,
               scene->vrect.size.h * scene->backingScale,
               0,
               GL_DEPTH_COMPONENT,
               GL_FLOAT,
               0);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
  
  glFramebufferTexture2D(GL_FRAMEBUFFER,
                         GL_DEPTH_ATTACHMENT,
                         GL_TEXTURE_2D,
                         pout->depth,
                         0);
  
   gkBindDefaultPassOut(scene);
}

GK_EXPORT
GkPassOutColor*
gkGetRenderTarget(GkPass *pass, int32_t index) {
  GkPassOut      *pout;
  GkPassOutColor *poc;

  if (!(pout = pass->output) || !(poc = pout->color))
    return NULL;

  while (index > 0) {
    poc = poc->next;
    index--;
  }

  return poc;
}

GK_EXPORT
void
gkBindRenderTargetToTexUnit(GkPassOutColor *rt, int32_t texUnit) {
  glActiveTexture(GL_TEXTURE0 + texUnit);
  glBindTexture(GL_TEXTURE_2D, rt->buffId);
}

GK_EXPORT
void
gkBindRenderTargetToTex(GkScene    *scene,
                        GkPass     *pass,
                        int32_t     targetIndex,
                        GkProgram  *prog,
                        int32_t     texUnit,
                        const char *uniformName) {
  /* get render target from pass output, then bind it to texture unit */
  gkBindRenderTargetToTexUnit(gkGetRenderTarget(pass, targetIndex),
                              texUnit);

  /* uniform texture unit to program */
  gkUniform1i(prog, uniformName, texUnit);
}

GK_EXPORT
void
gkBindDepthTexTo(GkScene    *scene,
                 GkPass     *pass,
                 GkProgram  *prog,
                 int32_t     texUnit,
                 const char  *uniformName) {
  /* then bind it to texture unit */
  glActiveTexture(GL_TEXTURE0 + texUnit);
  glBindTexture(GL_TEXTURE_2D, pass->output->depth);
  
  /* uniform texture unit to program */
  gkUniform1i(prog, uniformName, texUnit);
}

GK_EXPORT
GLuint
gkAddRenderTarget(GkScene *scene,
                  GkPass  *pass,
                  GLenum   format) {
  return gkAddRenderTargetEx(scene,
                             pass,
                             scene->internalFormat,
                             format,
                             scene->vrect.size.w * scene->backingScale,
                             scene->vrect.size.h * scene->backingScale,
                             GL_UNSIGNED_BYTE);
}

GK_EXPORT
GLuint
gkAddRenderTargetRB(struct GkScene *scene,
                    GkPass         *pass) {
  return gkAddRenderTargetRBEx(scene,
                               pass,
                               scene->internalFormat,
                               scene->vrect.size.w * scene->backingScale,
                               scene->vrect.size.h * scene->backingScale);
}

GLuint
gkAddRenderTargetRBEx(GkScene *scene,
                      GkPass  *pass,
                      GLenum   internalFormat,
                      GLsizei  width,
                      GLsizei  height) {
  GkPassOutColor *poc, *last_poc;
  GkPassOut      *pout;
  GLenum         *drawBuffs;
  int32_t         i;

  if (!(pout = pass->output))
    pass->output = pout = gkAllocPassOut();

  /* TODO:
   GLint maxAttach = 0;
   glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxAttach);

   do this when context initilized
   */

  if (pout->colorCount >= 15) {
    /* TODO: log and raise warning/errors */
    return 0;
  }

  last_poc = pout->color;
  poc = calloc(sizeof(*poc), 1);

  gkBindPassOut(scene, pout);

  glGenRenderbuffers(1, &poc->buffId);
  glBindRenderbuffer(GL_RENDERBUFFER, poc->buffId);

  glRenderbufferStorage(GL_RENDERBUFFER,
                        internalFormat,
                        width,
                        height);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                            GL_COLOR_ATTACHMENT0 + pout->colorCount,
                            GL_RENDERBUFFER,
                            poc->buffId);

  pout->colorCount++;

  drawBuffs = malloc(sizeof(GLenum) * pout->colorCount);
  for (i = 0; i < pout->colorCount; i++)
    drawBuffs[i] = GL_COLOR_ATTACHMENT0 + i;

  glDrawBuffers(pout->colorCount, drawBuffs);
  free(drawBuffs);

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    /* TODO: log and raise warning/errors */
  }

  if (last_poc) {
    while (last_poc->next) {
      last_poc = last_poc->next;
    }

    last_poc->next = poc;
  } else {
    pout->color = poc;
  }
  
  gkBindDefaultPassOut(scene);

  return pout->colorCount - 1;
}

GK_EXPORT
GLuint
gkAddRenderTargetEx(GkScene *scene,
                    GkPass  *pass,
                    GLenum   internalFormat,
                    GLenum   format,
                    GLsizei  width,
                    GLsizei  height,
                    GLenum   type) {
  GkPassOutColor *poc, *last_poc;
  GkPassOut      *pout;
  GLenum         *drawBuffs;
  int32_t         i;

  if (!(pout = pass->output))
    pass->output = pout = gkAllocPassOut();

  /* TODO:
       GLint maxAttach = 0;
       glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxAttach);

       do this when context initilized
   */

  if (pout->colorCount >= 15) {
    /* TODO: log and raise warning/errors */
    return 0;
  }

  last_poc = pout->color;
  poc = calloc(sizeof(*poc), 1);

  gkBindPassOut(scene, pout);

  glGenTextures(1, &poc->buffId);
  glBindTexture(GL_TEXTURE_2D, poc->buffId);

  glTexImage2D(GL_TEXTURE_2D,
               0,
               internalFormat,
               width,
               height,
               0,
               format,
               type,
               0);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glFramebufferTexture2D(GL_FRAMEBUFFER,
                         GL_COLOR_ATTACHMENT0,
                         GL_TEXTURE_2D,
                         poc->buffId,
                         0);

  pout->colorCount++;

  drawBuffs = malloc(sizeof(GLenum) * pout->colorCount);
  for (i = 0; i < pout->colorCount; i++)
    drawBuffs[i] = GL_COLOR_ATTACHMENT0 + i;

  glDrawBuffers(pout->colorCount, drawBuffs);
  free(drawBuffs);

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    /* TODO: log and raise warning/errors */
  }

  if (last_poc) {
    while (last_poc->next) {
      last_poc = last_poc->next;
    }

    last_poc->next = poc;
  } else {
    pout->color = poc;
  }

  gkBindDefaultPassOut(scene);

  return pout->colorCount - 1;
}
