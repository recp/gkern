/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../include/gk/gk.h"
#include "../../include/gk/pass.h"
#include "../../include/gk/scene.h"

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
gkBindPassOut(GkPassOut *pout) {
  glBindFramebuffer(GL_FRAMEBUFFER, pout->fbo);
}

GK_EXPORT
void
gkBindDefaultPassOut() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GK_EXPORT
GLuint
gkAddRenderTarget(GkScene *scene,
                  GkPass  *pass,
                  GLenum   format) {
  return gkAddRenderTargetEx(pass,
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
  return gkAddRenderTargetRBEx(pass,
                               scene->internalFormat,
                               scene->vrect.size.w * scene->backingScale,
                               scene->vrect.size.h * scene->backingScale);
}

GLuint
gkAddRenderTargetRBEx(GkPass *pass,
                      GLenum  internalFormat,
                      GLsizei width,
                      GLsizei height) {
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

  gkBindPassOut(pout);

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
  
  gkBindDefaultPassOut();

  return pout->colorCount - 1;
}

GK_EXPORT
GLuint
gkAddRenderTargetEx(GkPass *pass,
                    GLenum  internalFormat,
                    GLenum  format,
                    GLsizei width,
                    GLsizei height,
                    GLenum  type) {
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

  gkBindPassOut(pout);

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

  gkBindDefaultPassOut();

  return pout->colorCount - 1;
}
