/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "cmn_material.h"

#include "../../include/gk/gk.h"
#include "../../include/gk/material.h"
#include "../../include/gk/program.h"
#include "../../include/gk/vertex.h"

#include <ds/forward-list-sep.h>

#include <malloc/malloc.h>
#include <string.h>

static
void
gk__fillAttribs(GkColorOrTex * __restrict matAttribs[4],
                float       ** __restrict shininess,
                GkTechnique  * __restrict techn);

static
GkProgram*
gk_creatProgForCmnMat(char *name, void *userData);

size_t
gkShaderNameFor(GkScene     * __restrict scene,
                GkPrimitive * __restrict prim,
                GkTechnique * __restrict techn,
                char        * __restrict nameBuff) {
  char          *pname;
  GkColorOrTex  *attr[4];
  GkVertexInput *inp;
  FListItem     *inpi;
  int32_t        i;
  char           prefix[] = "dsaert";

  /*
   Shader Name: [TechniqueType]_[Inputs]_[Attribs]_[Extra...]
   */

  pname = nameBuff;
  
  gk__fillAttribs(attr, NULL, techn);
  sprintf(pname, "%d_", techn->type);
  
  /* primitive inputs */
  inpi = prim->inputs;
  while (inpi) {
    inp = inpi->data;
    const char *shortName;
    
    shortName = gkShortNameOfVertexInput(inp->name);
    if (!shortName)
      shortName = inp->name;

    pname += sprintf(pname, "%s", shortName);

    inpi = inpi->next;
  }

  /* primitive attributes, tasks */
  for (i = 0; i < 4; i++) {
    if (!attr[i])
      continue;

    pname += sprintf(pname, "%c%d", prefix[i], attr[i]->method);
  }
  
  if (GK_FLG(scene->flags, GK_SCENEF_SHADOWS))
    pname += sprintf(pname, "_shdw");

  /* TODO: transparent, reflectivity */
  return nameBuff - pname;
}

void
gkShaderFlagsFor(GkScene     * __restrict scene,
                 GkPrimitive * __restrict prim,
                 GkTechnique * __restrict techn,
                 char       ** __restrict vertFlags,
                 char       ** __restrict fragFlags) {
  char   *pFragFlags, *pVertFlags;
  float  *shininess;
  size_t  fragFlagsLen, vertFlagsLen;
  int32_t i, texCount;

  GkColorOrTex *attr[4];
  char *attrname[] = {
    "DIFFUSE",
    "SPECULAR",
    "AMBIENT",
    "EMISSION",
    "REFLECTIVE",
    "TRANSPARENT",
    "SHADOW_MAP"
  };

  fragFlagsLen = vertFlagsLen = PAGE_SIZE;
  *fragFlags   = malloc(fragFlagsLen);
  *vertFlags   = malloc(vertFlagsLen);
  pFragFlags   = *fragFlags;
  pVertFlags   = *vertFlags;

  pVertFlags[0] = pFragFlags[0] = '\0';

  shininess = NULL;
  gk__fillAttribs(attr, &shininess, techn);

  texCount = 0;
  for (i = 0; i < 4; i++) {
    ptrdiff_t pdiff;

    if (!attr[i])
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

    switch (attr[i]->method) {
      case GK_COLOR_COLOR:
        pFragFlags += sprintf(pFragFlags,
                              "\n#define %s_COLOR\n",
                              attrname[i]);
        break;
      case GK_COLOR_TEX:
        texCount++;
        pFragFlags += sprintf(pFragFlags,
                              "\n#define %s_TEX\n",
                              attrname[i]);
        break;
      default:
        continue;
    }
  }

  /* TODO: transparent, reflectivity */

  if (shininess) {
    pFragFlags += sprintf(pFragFlags, "\n#define SHININESS\n");
  }

  if (texCount > 0) {
    pVertFlags += sprintf(pVertFlags,
                          "\n#define TEX_COUNT %d\n",
                          texCount);

    pFragFlags += sprintf(pFragFlags,
                          "\n#define TEX_COUNT %d\n",
                          texCount);
  }
  
  if (GK_FLG(scene->flags, GK_SCENEF_SHADOWS)) {
    sprintf(pVertFlags, "\n#define SHADOW_MAP\n");
    sprintf(pFragFlags, "\n#define SHADOW_MAP\n");
  }
}

GkShader*
gkShadersFor(GkScene     * __restrict scene,
             GkPrimitive * __restrict prim,
             GkTechnique * __restrict techn) {
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

  gkShaderFlagsFor(scene,
                   prim,
                   techn,
                   &vertSource[1],
                   &fragSource[1]);

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

GkProgram*
gkGetOrCreatProgForCmnMat(GkScene     * __restrict scene,
                          GkPrimitive * __restrict prim,
                          GkMaterial  * __restrict mat) {
  char  name[64];
  void *userData[3];

  (void)gkShaderNameFor(scene,
                        prim,
                        mat->technique,
                        name);

  userData[0] = scene;
  userData[1] = prim;
  userData[2] = mat;
  
  return gkGetOrCreatProg(name,
                          gk_creatProgForCmnMat,
                          userData);
}

static
void
gk__beforeLinking(GkProgram *prog, void *data) {
  GkPrimitive   *prim;
  FListItem     *inpi;
  GkVertexInput *inp;
  int32_t        index;

  prim  = data;
  index = 0;
  inpi  = prim->inputs;
  while (inpi) {
    inp = inpi->data;
    
    glBindAttribLocation(prog->prog, index, inp->name);
    
    index++;
    inpi = inpi->next;
  }
}

static
GkProgram*
gk_creatProgForCmnMat(char *name, void *userData) {
  GkShader    *shaders;
  GkScene     *scene;
  GkPrimitive *prim;
  GkMaterial  *mat;

  scene = ((void **)userData)[0];
  prim  = ((void **)userData)[1];
  mat   = ((void **)userData)[2];
  
  if ((shaders = gkShadersFor(scene, prim, mat->technique)))
    return gkMakeProgram(shaders, gk__beforeLinking, prim);

  return NULL;
}

static
void
gk__fillAttribs(GkColorOrTex * __restrict matAttribs[4],
                float       ** __restrict shininess,
                GkTechnique  * __restrict techn) {
  switch (techn->type) {
    case GK_MATERIAL_PHONG:
    case GK_MATERIAL_BLINN: {
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
