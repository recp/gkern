/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "../../include/gk/gk.h"
#include "../../include/gk/pass.h"
#include "../../include/gk/scene.h"
#include "../state/gpu.h"

GK_EXPORT
GkPass*
gkAllocPass(void) {
  GkPass *pass;

  pass         = calloc(1, sizeof(*pass));
  pass->output = gkAllocOutput();

  return pass;
}

GK_EXPORT
GkOutput*
gkAllocOutput(void) {
  GkOutput *output;

  output = calloc(1, sizeof(*output));
  glGenFramebuffers(1, &output->fbo);

  return output;
}

GK_EXPORT
GkOutput*
gkCurrentOutput(GkContext * __restrict ctx) {
  GkOutput *output;

  if ((output = ctx->currState->outputState->renderOutput))
    return output;

  return gkDefaultRenderOut();
}

GK_EXPORT
void
gkBindOutput(GkScene  *scene,
              GkOutput *output) {
  GkOutputState *state;
  GkContext     *ctx;

  ctx   = gkContextOf(scene);
  state = gkGetOrCreatState(ctx, GK_GPUSTATE_OUTPUT);
  if (ctx->currState->outputState->renderOutput == output)
    return;

  state->renderOutput = output;
  glBindFramebuffer(GL_FRAMEBUFFER, output->fbo);
}

GK_EXPORT
void
gkBindDefaultOutput(GkScene *scene) {
  GkOutputState *state;
  GkContext        *ctx;

  ctx   = gkContextOf(scene);
  state = gkGetOrCreatState(ctx, GK_GPUSTATE_OUTPUT);
  if (ctx->currState->outputState->renderOutput == NULL)
    return;

  state->renderOutput = NULL;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GK_EXPORT
void
gkPassEnableDepth(GkScene *scene,
                  GkPass  *pass) {
  GkOutput *output;

  if (!(output = pass->output))
    pass->output = output = gkAllocOutput();

  if (output->depth != 0)
    return;

  glGenRenderbuffers(1, &output->depth);
  glBindRenderbuffer(GL_RENDERBUFFER, output->depth);

  glRenderbufferStorage(GL_RENDERBUFFER,
                        GL_DEPTH_COMPONENT24,
                        scene->vrect.size.w * scene->backingScale,
                        scene->vrect.size.h * scene->backingScale);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                            GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER,
                            output->depth);
}

GK_EXPORT
void
gkPassEnableDepthTex(GkScene *scene,
                     GkPass  *pass,
                     GkSize   size) {
  GkOutput *output, *currentOutput;

  if (!(output = pass->output))
    pass->output = output = gkAllocOutput();

  if (output->depth != 0)
    return;

  currentOutput = gkCurrentOutput(gkContextOf(scene));
  if (currentOutput != output)
    gkBindOutput(scene, output);

  glGenTextures(1, &output->depth);
  glBindTexture(GL_TEXTURE_2D, output->depth);

  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_DEPTH_COMPONENT24, /* todo: add option for this */
               size.w,
               size.h,
               0,
               GL_DEPTH_COMPONENT,
               GL_FLOAT,
               NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,   GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,   GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,       GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,       GL_CLAMP_TO_EDGE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_COMPARE_MODE,
                  GL_COMPARE_REF_TO_TEXTURE);

  glFramebufferTexture2D(GL_FRAMEBUFFER,
                         GL_DEPTH_ATTACHMENT,
                         GL_TEXTURE_2D,
                         output->depth,
                         0);

  if (currentOutput != output)
    gkBindOutput(scene, currentOutput);
}

GK_EXPORT
void
gkPassEnableDepthCubeTex(GkScene *scene, GkPass *pass, float size) {
  GkOutput *output, *currentOutput;
  int       i;

  if (!(output = pass->output))
    pass->output = output = gkAllocOutput();

  if (output->depth != 0)
    return;

  currentOutput = gkCurrentOutput(gkContextOf(scene));
  if (currentOutput != output)
    gkBindOutput(scene, output);

  glGenTextures(1, &output->depth);
  glBindTexture(GL_TEXTURE_CUBE_MAP, output->depth);

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,
                  GL_TEXTURE_COMPARE_MODE,
                  GL_COMPARE_REF_TO_TEXTURE);

  for (i = 0; i < 6; i++)
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                 0,
                 GL_DEPTH_COMPONENT24,
                 size,
                 size,
                 0,
                 GL_DEPTH_COMPONENT,
                 GL_FLOAT,
                 NULL);

  if (currentOutput != output)
    gkBindOutput(scene, currentOutput);
}

GK_EXPORT
void
gkPassEnableDepthTexArray(GkScene *scene, GkPass *pass, GLsizei len) {
  GkOutput *output, *currentOutput;

  if (!(output = pass->output))
    pass->output = output = gkAllocOutput();

  if (output->depth != 0)
    return;

  currentOutput = gkCurrentOutput(gkContextOf(scene));
  if (currentOutput != output)
    gkBindOutput(scene, output);

  glGenTextures(1, &output->depth);
  glBindTexture(GL_TEXTURE_2D_ARRAY, output->depth);

  glTexImage3D(GL_TEXTURE_2D_ARRAY,
               0,
               GL_DEPTH_COMPONENT24, /* todo: add option for this */
               scene->vrect.size.w * scene->backingScale,
               scene->vrect.size.h * scene->backingScale,
               len,
               0,
               GL_DEPTH_COMPONENT,
               GL_FLOAT,
               NULL);

  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameteri(GL_TEXTURE_2D_ARRAY,
                  GL_TEXTURE_COMPARE_MODE,
                  GL_COMPARE_REF_TO_TEXTURE);

  glFramebufferTextureLayer(GL_FRAMEBUFFER,
                            GL_DEPTH_ATTACHMENT,
                            output->depth,
                            0,
                            0);

  if (currentOutput != output)
    gkBindOutput(scene, currentOutput);
}

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
gkBindRenderTargetToTexUnit(GkColorOutput *rt, int32_t texUnit) {
  glActiveTexture(GL_TEXTURE0 + texUnit);
  glBindTexture(GL_TEXTURE_2D, rt->buffId);
}

GK_EXPORT
void
gkBindRenderTargetTo(GkScene    *scene,
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
void
gkBindDepthTexArrayTo(GkScene    *scene,
                      GkPass     *pass,
                      GkProgram  *prog,
                      int32_t     texUnit,
                      const char  *uniformName) {
  /* then bind it to texture unit */
  glActiveTexture(GL_TEXTURE0 + texUnit);
  glBindTexture(GL_TEXTURE_2D_ARRAY, pass->output->depth);

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
    pass->output = output = gkAllocOutput();

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
    pass->output = output = gkAllocOutput();

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
