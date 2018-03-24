/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../common.h"
#include "../../../include/gk/gk.h"
#include "../../../include/gk/pass.h"
#include "../../../include/gk/scene.h"
#include "../../state/gpu.h"

GK_EXPORT
GkColorOutput*
gkGetRenderTarget(GkPass *pass, int32_t index) {
  GkOutput      *output;
  GkColorOutput *poc;

  if (!(output = pass->output) || !(poc = output->color))
    return NULL;

  while (index > 0) {
    poc = poc->next;
    index--;
  }

  return poc;
}

GK_EXPORT
void
gkBindRenderTargetTo(GkScene    *scene,
                     GkPass     *pass,
                     int32_t     targetIndex,
                     GkProgram  *prog,
                     int32_t     texUnit,
                     const char *uniformName) {
  GkContext     *ctx;
  GkColorOutput *rt;

  ctx = gkContextOf(scene);
  rt  = gkGetRenderTarget(pass, targetIndex);

  gkBindTextureTo(ctx, texUnit, GL_TEXTURE_2D, rt->buffId);

  /* uniform texture unit to program */
  gkUniform1i(prog, uniformName, texUnit);
}

GK_EXPORT
GLuint
gkAddRenderTarget(GkScene *scene,
                  GkPass  *pass,
                  GLenum   internalformat,
                  GLenum   format,
                  GLenum   type) {
  return gkAddRenderTargetEx(scene,
                             pass,
                             internalformat,
                             format,
                             scene->vrect.size.w * scene->backingScale,
                             scene->vrect.size.h * scene->backingScale,
                             type);
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
  GkColorOutput *poc, *last_poc;
  GkOutput      *output, *currentOutput;
  GLenum        *drawBuffs;
  int32_t        i;

  if (!(output = pass->output))
    pass->output = output = gkAllocOutput(gkContextOf(scene));

  /* TODO:
   GLint maxAttach = 0;
   glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxAttach);

   do this when context initilized
   */

  if (output->colorCount >= 15) {
    /* TODO: log and raise warning/errors */
    return 0;
  }

  last_poc = output->color;
  poc = calloc(1, sizeof(*poc));

  currentOutput = gkCurrentOutput(gkContextOf(scene));
  if (currentOutput != output)
    gkBindOutput(scene, output);

  glGenRenderbuffers(1, &poc->buffId);
  glBindRenderbuffer(GL_RENDERBUFFER, poc->buffId);

  glRenderbufferStorage(GL_RENDERBUFFER,
                        internalFormat,
                        width,
                        height);

  poc->attachment = GL_COLOR_ATTACHMENT0 + output->colorCount;

  glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                            poc->attachment,
                            GL_RENDERBUFFER,
                            poc->buffId);

  poc->drawIndex = output->colorCount++;

  drawBuffs = malloc(sizeof(GLenum) * output->colorCount);
  for (i = 0; i < output->colorCount; i++)
    drawBuffs[i] = GL_COLOR_ATTACHMENT0 + i;

  glDrawBuffers(output->colorCount, drawBuffs);
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
    output->color = poc;
  }

  if (currentOutput != output)
    gkBindOutput(scene, currentOutput);

  return output->colorCount - 1;
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
  GkColorOutput *poc, *last_poc;
  GkOutput      *output, *currentOutput;
  GLenum        *drawBuffs;
  int32_t        i;

  if (!(output = pass->output))
    pass->output = output = gkAllocOutput(gkContextOf(scene));

  /* TODO:
   GLint maxAttach = 0;
   glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxAttach);

   do this when context initilized
   */

  if (output->colorCount >= 15) {
    /* TODO: log and raise warning/errors */
    return 0;
  }

  last_poc = output->color;
  poc = calloc(1, sizeof(*poc));

  currentOutput = gkCurrentOutput(gkContextOf(scene));
  if (currentOutput != output)
    gkBindOutput(scene, output);

  glGenTextures(1, &poc->buffId);
  glBindTexture(GL_TEXTURE_2D, poc->buffId);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, width, height);


  poc->attachment = GL_COLOR_ATTACHMENT0 + output->colorCount;

  glFramebufferTexture2D(GL_FRAMEBUFFER,
                         poc->attachment,
                         GL_TEXTURE_2D,
                         poc->buffId,
                         0);

  poc->drawIndex = output->colorCount++;

  drawBuffs = malloc(sizeof(GLenum) * output->colorCount);
  for (i = 0; i < output->colorCount; i++)
    drawBuffs[i] = GL_COLOR_ATTACHMENT0 + i;

  glDrawBuffers(output->colorCount, drawBuffs);
  free(drawBuffs);

  GK_DEBUG_GPU_FRAMEBUFF_ASSERT_ONERROR();

  if (last_poc) {
    while (last_poc->next) {
      last_poc = last_poc->next;
    }

    last_poc->next = poc;
  } else {
    output->color = poc;
  }

  if (currentOutput != output)
    gkBindOutput(scene, currentOutput);

  return output->colorCount - 1;
}

