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
void
gkAddDepthTarget(GkScene *scene, GkPass *pass) {
  GkOutput *output;

  if (!(output = pass->output))
    pass->output = output = gkAllocOutput(gkContextOf(scene));

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
gkAddDepthTexTarget(GkScene *scene, GkPass *pass, GkSize size) {
  GkOutput *output, *currentOutput;

  if (!(output = pass->output))
    pass->output = output = gkAllocOutput(gkContextOf(scene));

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
gkAddDepthCubeTexTarget(GkScene *scene, GkPass *pass, float size) {
  GkOutput *output, *currentOutput;
  int       i;

  if (!(output = pass->output))
    pass->output = output = gkAllocOutput(gkContextOf(scene));

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
gkAddDepthTexArrayTarget(GkScene *scene, GkPass *pass, GLsizei len) {
  GkOutput *output, *currentOutput;

  if (!(output = pass->output))
    pass->output = output = gkAllocOutput(gkContextOf(scene));

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
void
gkBindDepthTexTo(GkScene    *scene,
                 GkPass     *pass,
                 GkProgram  *prog,
                 int32_t     texUnit,
                 const char  *uniformName) {
  gkBindTextureTo(gkContextOf(scene),
                  texUnit,
                  GL_TEXTURE_2D,
                  pass->output->depth);

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
  gkBindTextureTo(gkContextOf(scene),
                  texUnit,
                  GL_TEXTURE_2D_ARRAY,
                  pass->output->depth);

  /* uniform texture unit to program */
  gkUniform1i(prog, uniformName, texUnit);
}
