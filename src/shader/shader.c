/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "shader.h"

#include "../../include/gk/shader.h"
#include "../../include/gk/material.h"
#include <ds/rb.h>

static RBTree *gk_shaders;

static
GkPipeline*
gk_creatProgByName(char *name, void *userData);

GkShader*
gkShaderByName(const char *name) {
  return rb_find(gk_shaders, (void *)name);
}

void
gkShaderSetName(GkShader *shader, const char *name) {
  void *found;
  if ((found = rb_find(gk_shaders, (void *)name)))
    rb_remove(gk_shaders, (void *)name);

  rb_insert(gk_shaders, (void *)name, shader);
}

GkPipeline*
gkGetOrCreatProgByName(const char   *commonName,
                       const char   *shaderSources[],
                       GLenum        shaderTypes[],
                       uint32_t      shaderCount,
                       GkShaderFlags flags) {
  void **paramStructure[4];

  paramStructure[0] = (void *)shaderSources;
  paramStructure[1] = (void *)shaderTypes;
  paramStructure[2] = (void *)&shaderCount;
  paramStructure[3] = (void *)&flags;

  return gkGetPipeline((char *)commonName,
                          gk_creatProgByName,
                          paramStructure);
}

static
GkPipeline*
gk_creatProgByName(char *name, void *userData) {
  void        **paramStructure;
  const char  **shaderSources;
  GLenum       *shaderTypes;
  GkPipeline    *prog;
  GkShader     *last_shader;
  GLuint        progId;
  GkShaderFlags flags;
  uint32_t      shaderCount;
  int32_t       i;

  paramStructure = (void **)userData;
  shaderSources  = paramStructure[0];
  shaderTypes    = paramStructure[1];
  shaderCount    = *(uint32_t *)paramStructure[2];
  flags          = *(GkShaderFlags *)paramStructure[3];

  prog          = calloc(1, sizeof(*prog));
  progId        = glCreateProgram();
  last_shader   = NULL;

  for (i = 0; i < shaderCount; i++) {
    GkShader *shader;
    char     *source[2];

    /* TODO: create dynamic by platform */
    source[0] = "#version 410 \n";
    source[1] = (char *)shaderSources[i];

    shader = calloc(1, sizeof(*shader));
    shader->isValid    = 1;
    shader->shaderType = shaderTypes[i];

    shader->shaderId   = gkShaderLoadN(shaderTypes[i],
                                       source,
                                       2);

    glAttachShader(progId, shader->shaderId);

    if (last_shader)
      last_shader->next = shader;
    else
      prog->shaders = shader;
    last_shader = shader;
  }

  glLinkProgram(progId);

#ifdef DEBUG
  if (!gkProgramIsValid(progId)) {
    gkProgramLogInfo(progId, stderr);
    exit(-1);
  }
#endif

  prog->mvpi = prog->mvi = prog->nmi = prog->nmui = prog->vpi = -1;

  if (GK_FLG(flags, GK_SHADER_FLAG_MVP))
    prog->mvpi = glGetUniformLocation(progId, "MVP");
  if (GK_FLG(flags, GK_SHADER_FLAG_MV))
    prog->mvi  = glGetUniformLocation(progId, "MV");
  if (GK_FLG(flags, GK_SHADER_FLAG_NM))
    prog->nmi  = glGetUniformLocation(progId, "NM");
  if (GK_FLG(flags, GK_SHADER_FLAG_NMU))
    prog->nmui = glGetUniformLocation(progId, "NMU");
  if (GK_FLG(flags, GK_SHADER_FLAG_VP))
    prog->vpi  = glGetUniformLocation(progId, "VP");

  prog->progId        = progId;
  prog->refc          = 1;
  prog->updtLights    = 1;
  prog->updtMaterials = 1;

  return prog;
}

void
gk_shaders_init() {
  gk_shaders = rb_newtree_str();
}

void
gk_shaders_deinit() {
  rb_destroy(gk_shaders);
}
