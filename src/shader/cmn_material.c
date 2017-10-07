/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../include/gk/gk.h"
#include "../../include/gk/material.h"
#include "../../include/gk/program.h"
#include <string.h>

#include "cmn_material.h"
#include <malloc/malloc.h>

static
void
gk__fillAttribs(GkColorOrTex * __restrict matAttribs[4],
                float       ** __restrict shininess,
                GkTechnique  * __restrict techn);

static
GkProgInfo*
gk_creatProgForCmnMat(char *name, void *userData);

size_t
gkShaderNameFor(GkTechnique * __restrict techn,
                char        * __restrict nameBuff,
                char        * __restrict prefix) {
  char         *pname;
  GkColorOrTex *matAttribs[4];
  int32_t       i;

  char matAttribsPrefix[] = "dsaert";

  gk__fillAttribs(matAttribs, NULL, techn);

  pname  = nameBuff;

  if (prefix) {
    pname += sprintf(nameBuff,
                     "%s%d_",
                     prefix,
                     techn->type);
  } else {
    pname += sprintf(nameBuff,
                     "%d_",
                     techn->type);
  }

  for (i = 0; i < 4; i++) {
    if (!matAttribs[i])
      continue;

    pname += sprintf(pname,
                     "%c%d",
                     matAttribsPrefix[i],
                     matAttribs[i]->method);
  }

  /* TODO: transparent, reflectivity */
  return nameBuff - pname;
}

void
gkShaderFlagsFor(GkTechnique * __restrict techn,
                 char       ** __restrict vertFlags,
                 char       ** __restrict fragFlags) {
  char   *pFragFlags, *pVertFlags;
  float  *shininess;
  size_t  fragFlagsLen, vertFlagsLen;
  int32_t i, texCount;

  GkColorOrTex *matAttribs[4];
  char *matAttribsNames[] = {
    "DIFFUSE",
    "SPECULAR",
    "AMBIENT",
    "EMISSION",
    "REFLECTIVE",
    "TRANSPARENT"
  };

  fragFlagsLen = vertFlagsLen = PAGE_SIZE;
  *fragFlags   = malloc(fragFlagsLen);
  *vertFlags   = malloc(vertFlagsLen);
  pFragFlags   = *fragFlags;
  pVertFlags   = *vertFlags;

  pVertFlags[0] = pFragFlags[0] = '\0';

  shininess = NULL;
  gk__fillAttribs(matAttribs, &shininess, techn);

  texCount = 0;
  for (i = 0; i < 4; i++) {
    ptrdiff_t pdiff;

    if (!matAttribs[i])
      continue;

    pdiff = pFragFlags - *fragFlags;
    if (pFragFlags != *fragFlags && pdiff < 64) {
      *fragFlags = realloc(*fragFlags, fragFlagsLen + 256);
      fragFlagsLen += 256;
      pFragFlags = *fragFlags + pdiff;
    }

    pdiff = pVertFlags - *vertFlags;
    if (pVertFlags != *vertFlags && pdiff < 64) {
      *vertFlags = realloc(*vertFlags, vertFlagsLen + 256);
      vertFlagsLen += 256;
      pVertFlags = *vertFlags + pdiff;
    }

    switch (matAttribs[i]->method) {
      case GK_COLOR_COLOR:
        pFragFlags += sprintf(pFragFlags,
                              "\n#define %s_COLOR\n",
                              matAttribsNames[i]);
        break;
      case GK_COLOR_TEX:
        texCount++;
        pFragFlags += sprintf(pFragFlags,
                              "\n#define %s_TEX\n",
                              matAttribsNames[i]);
        break;
      default:
        continue;
    }
  }

  /* TODO: transparent, reflectivity */

  if (shininess) {
    pFragFlags = strcpy(pFragFlags, "\n#define SHININESS\n");
  }

  if (texCount > 0) {
    sprintf(pFragFlags,
            "\n#define TEX_COUNT %d\n",
            texCount);

   sprintf(pVertFlags,
           "\n#define TEX_COUNT %d\n",
           texCount);
  }
}

GkShader*
gkShadersFor(GkTechnique * __restrict techn) {
  GkShader *vert, *frag;
  char     *fragSource[5], *vertSource[3];

  /* TODO: create dynamic by platform */
  vertSource[0] = fragSource[0] = "\n#version 410 \n";

  switch (techn->type) {
    case GK_MATERIAL_PHONG:
      fragSource[4] =
#include "glsl/frag/phong.glsl"
      ;
      break;
    case GK_MATERIAL_BLINN:
      fragSource[4] =
#include "glsl/frag/blinn.glsl"
      ;
      break;
    case GK_MATERIAL_LAMBERT:
      fragSource[4] =
#include "glsl/frag/lambert.glsl"
      ;
      break;
    case GK_MATERIAL_CONSTANT:
      fragSource[4] =
#include "glsl/frag/constant.glsl"
      ;
      break;
    default:
      return NULL;
  }

  fragSource[2] =
#include "glsl/frag/common.glsl"
  ;

  fragSource[3] =
#include "glsl/frag/lights.glsl"
  ;

  gkShaderFlagsFor(techn, &vertSource[1], &fragSource[1]);

  vert = (GkShader *)calloc(sizeof(*vert), 1);
  vert->isValid    = 1;
  vert->shaderType = GL_VERTEX_SHADER;

  vertSource[2] =
#include "glsl/vert/common.glsl"
  ;

  vert->shaderId = gkShaderLoadN(vert->shaderType,
                                 vertSource,
                                 3);

  frag = (GkShader *)calloc(sizeof(*frag), 1);
  frag->isValid    = 1;
  frag->shaderType = GL_FRAGMENT_SHADER;
  frag->shaderId   = gkShaderLoadN(frag->shaderType,
                                   fragSource,
                                   5);

  vert->next = frag;

  return vert;
}

GkProgInfo*
gkGetOrCreatProgForCmnMat(GkMaterial *mat) {
  char name[64];

  (void)gkShaderNameFor(mat->technique, name, NULL);

  return gkGetOrCreatProg(name,
                          gk_creatProgForCmnMat,
                          mat);
}

static
GkProgInfo*
gk_creatProgForCmnMat(char *name, void *userData) {
  GkShader   *shaders;
  GkMaterial *mat;

  mat = userData;
  if ((shaders = gkShadersFor(mat->technique)))
    return gkNewProgram(shaders);

  return NULL;
}

static
void
gk__fillAttribs(GkColorOrTex * __restrict matAttribs[4],
                float       ** __restrict shininess,
                GkTechnique  * __restrict techn) {
  switch (techn->type) {
    case GK_MATERIAL_PHONG:
    case GK_MATERIAL_BLINN:{
      GkPhong *phong;
      phong = (GkPhong *)techn;
      matAttribs[0] = phong->diffuse;
      matAttribs[1] = phong->specular;
      matAttribs[2] = phong->ambient;
      matAttribs[3] = phong->emission;

      if (shininess)
        *shininess = &phong->shininess;
      break;
    }
    case GK_MATERIAL_LAMBERT: {
      GkLambert *lambert;
      lambert = (GkLambert *)techn;
      matAttribs[0] = lambert->diffuse;
      matAttribs[1] = NULL;
      matAttribs[2] = lambert->ambient;
      matAttribs[3] = lambert->emission;
      break;
    }
    case GK_MATERIAL_CONSTANT: {
      GkConstant *constant;
      constant = (GkConstant *)techn;
      matAttribs[0] = NULL;
      matAttribs[1] = NULL;
      matAttribs[2] = NULL;
      matAttribs[3] = constant->emission;
      break;
    }

    default:
      return;
  }
}

