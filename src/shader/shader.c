/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "shader.h"

#include "../../include/gk/shader.h"
#include "../../include/gk/material.h"
#include <ds/rb.h>

static RBTree *gk_shaders;

static
GkProgInfo*
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

GkProgInfo*
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

  return gkGetOrCreatProg((char *)commonName,
                          gk_creatProgByName,
                          paramStructure);
}

static
GkProgInfo*
gk_creatProgByName(char *name, void *userData) {
  void      **paramStructure;
  const char  **shaderSources;
  GLenum       *shaderTypes;
  GkProgInfo   *pinfo;
  GkShader     *last_shader;
  GLuint        program;
  GkShaderFlags flags;
  uint32_t      shaderCount;
  int32_t       i;

  paramStructure = (void **)userData;
  shaderSources  = paramStructure[0];
  shaderTypes    = paramStructure[1];
  shaderCount    = *(uint32_t *)paramStructure[2];
  flags          = *(GkShaderFlags *)paramStructure[3];

  pinfo         = calloc(sizeof(*pinfo), 1);
  program       = glCreateProgram();
  last_shader   = NULL;

  for (i = 0; i < shaderCount; i++) {
    GkShader *shader;
    char     *source[2];

    /* TODO: create dynamic by platform */
    source[0] = "#version 410 \n";
    source[1] = (char *)shaderSources[i];

    shader = calloc(sizeof(*shader), 1);
    shader->isValid    = 1;
    shader->shaderType = shaderTypes[i];

    shader->shaderId   = gkShaderLoadN(shaderTypes[i],
                                       source,
                                       2);

    glAttachShader(program, shader->shaderId);

    if (last_shader)
      last_shader->next = shader;
    else
      pinfo->shaders = shader;
    last_shader = shader;
  }

  glLinkProgram(program);

#ifdef DEBUG
  if (!gkProgramIsValid(program)) {
    gkProgramLogInfo(program, stderr);
    exit(-1);
  }
#endif

  if (GK_FLG(flags, GK_SHADER_FLAG_MVP))
    pinfo->mvpi = glGetUniformLocation(program, "MVP");
  if (GK_FLG(flags, GK_SHADER_FLAG_MV))
    pinfo->mvi  = glGetUniformLocation(program, "MV");
  if (GK_FLG(flags, GK_SHADER_FLAG_NM))
    pinfo->nmi  = glGetUniformLocation(program, "NM");
  if (GK_FLG(flags, GK_SHADER_FLAG_NMU))
    pinfo->nmui = glGetUniformLocation(program, "NMU");

  pinfo->prog          = program;
  pinfo->refc          = 1;
  pinfo->updtLights    = 1;
  pinfo->updtMaterials = 1;

  return pinfo;
}

void
gk_shaders_init() {
  gk_shaders = rb_newtree_str();
}

void
gk_shaders_deinit() {
  rb_destroy(gk_shaders);
}
