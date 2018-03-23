/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "cmn_material.h"

#include "../../include/gk/gk.h"
#include "../../include/gk/material.h"
#include "../../include/gk/program.h"
#include "../../include/gk/vertex.h"
#include "../../include/gk/shadows.h"
#include "../../include/gk/transparent.h"
#include "../render/realtime/rn_transp.h"
#include <ds/forward-list-sep.h>

#include <malloc/malloc.h>
#include <string.h>

#define _DF(X)  "\n#define " X "\n"
#define SH_V_ARG(F, ...) flg->vert += sprintf(flg->vert, _DF(F), __VA_ARGS__);
#define SH_V(F, ...)     flg->vert += sprintf(flg->vert, _DF(F));
#define SH_F_ARG(F, ...) flg->frag += sprintf(flg->frag, _DF(F), __VA_ARGS__);
#define SH_F(F, ...)     flg->frag += sprintf(flg->frag, _DF(F));

#define SH_VF_ARG(F, ...) \
  do { \
    flg->vert += sprintf(flg->vert, _DF(F), __VA_ARGS__); \
    flg->frag += sprintf(flg->frag, _DF(F), __VA_ARGS__); \
  } while(0);

#define SH_VF(F) \
  do { \
    flg->vert += sprintf(flg->vert, _DF(F)); \
    flg->frag += sprintf(flg->frag, _DF(F)); \
  } while (0);

typedef struct GkFlagsStruct {
  char *vert;
  char *frag;
  int   texCount;
} GkFlagsStruct;

static
void
gk__texFlag(GkTexture     * __restrict tex,
            char          * __restrict attrname,
            GkFlagsStruct * __restrict flags);

static
void
gk__colorOrTexFlag(GkColorDesc   * __restrict attr,
                   char          * __restrict attrname,
                   GkFlagsStruct * __restrict flags);

static
void
gk__fillAttribs(GkMaterial  * __restrict mat,
                GkColorDesc * __restrict matAttribs[4],
                GkTechnique * __restrict techn);

static
GkProgram*
gk_creatProgForCmnMat(char *name, void *userData);

size_t
gkShaderNameFor(GkScene     * __restrict scene,
                GkLight     * __restrict light,
                GkPrimitive * __restrict prim,
                GkMaterial  * __restrict mat,
                char        * __restrict nameBuff) {
  GkTechnique   *techn;
  char          *pname;
  GkColorDesc  *attr[4];
  GkVertexInput *inp;
  FListItem     *inpi;
  int32_t        i;
  char           prefix[] = "dsaert";

  /*
   Shader Name: [TechniqueType]_[Inputs]_[Attribs]_[Extra...]
   */

  pname = nameBuff;
  techn = mat->technique;

  memset(attr, 0, sizeof(attr));
  gk__fillAttribs(mat, attr, techn);
  pname += sprintf(pname, "%d_", techn->type);

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

  /* Occlusion Map */
  if (techn->occlusion && techn->occlusion->tex)
    pname += sprintf(pname, "_oc");

  /* Normal Map */
  if (techn->normal && techn->normal->tex)
    pname += sprintf(pname, "_n");

  /* PBR flags */
  switch (techn->type) {
    case GK_MATERIAL_METALROUGH: {
      GkMetalRough *metalRough;

      metalRough = (GkMetalRough *)techn;

      if (metalRough->albedoMap)
        pname += sprintf(pname, "_a");

      if (metalRough->metalRoughMap)
        pname += sprintf(pname, "_mr");

      break;
    }

    case GK_MATERIAL_SPECGLOSS: {
      GkSpecGloss *specGloss;

      specGloss = (GkSpecGloss *)techn;

      if (specGloss->diffuseMap)
        pname += sprintf(pname, "_d");

      if (specGloss->specGlossMap)
        pname += sprintf(pname, "_sg");
      break;
    }

    default: break;
  }

  /* primitive attributes, tasks */
  for (i = 0; i < 4; i++) {
    if (!attr[i])
      continue;

    pname += sprintf(pname, "%c%d", prefix[i], attr[i]->method);
  }

  if (GK_FLG(scene->flags, GK_SCENEF_SHADOWS))
    pname += sprintf(pname, "_shdw");

  if (gkIsTransparent(scene, mat)) {
    switch (gkTranspTechn()) {
      case GK_TRANSP_WEIGHTED_BLENDED:
        pname += sprintf(pname, "_trsp_wbl");
        break;
      default:
        pname += sprintf(pname, "_trsp");
        break;
    }
  }

  /* TODO: transparent, reflectivity */
  return nameBuff - pname;
}

void
gkShaderFlagsFor(GkScene     * __restrict scene,
                 GkLight     * __restrict light,
                 GkPrimitive * __restrict prim,
                 GkMaterial  * __restrict mat,
                 char       ** __restrict vertFlags,
                 char       ** __restrict fragFlags) {
  GkTechnique  *tech;
  int32_t       i;
  GkFlagsStruct flags, *flg;

  GkColorDesc *attr[4];
  char *attrname[] = {
    "DIFFUSE",
    "SPECULAR",
    "AMBIENT",
    "EMISSION",
    "REFLECTIVE",
    "TRANSP",
    "SHADOWMAP"
  };

  *fragFlags     = malloc(PAGE_SIZE);
  *vertFlags     = malloc(PAGE_SIZE);
  flags.frag     = *fragFlags;
  flags.vert     = *vertFlags;
  tech           = mat->technique;
  flg            = &flags;

  flags.frag[0]  = flags.vert[0] = '\0';
  flags.texCount = 0;

  memset(attr, 0, sizeof(attr));
  gk__fillAttribs(mat, attr, tech);

  /* Common profile: Color or Texture flags */
  for (i = 0; i < 4; i++) {
    if (!attr[i])
      continue;

    gk__colorOrTexFlag(attr[i], attrname[i], flg);
  }

  /* Occlusion Map */
  if (tech->occlusion && tech->occlusion->tex) {
    gk__texFlag(tech->occlusion->tex, "OCCLUSION", flg);
  }

  /* Normal Map */
  if (tech->normal && tech->normal->tex) {
    gk__texFlag(tech->normal->tex, "NORMAL", flg);
  }

  /* TODO: reflectivity */

  if (tech->type == GK_MATERIAL_PHONG || tech->type == GK_MATERIAL_BLINN)
    SH_F("SHININESS");

  /* PBR flags */
  switch (tech->type) {
    case GK_MATERIAL_METALROUGH: {
      GkMetalRough *metalRough;

      metalRough = (GkMetalRough *)tech;
      if (metalRough->albedoMap)
        gk__texFlag(metalRough->albedoMap, "ALBEDO", flg);

      if (metalRough->metalRoughMap)
        gk__texFlag(metalRough->metalRoughMap, "METALROUGH", flg);

      break;
    }
    case GK_MATERIAL_SPECGLOSS: {
      GkSpecGloss *specGloss;

      specGloss = (GkSpecGloss *)tech;
      if (specGloss->diffuseMap)
        gk__texFlag(specGloss->diffuseMap, "DIFFUSE", flg);

      if (specGloss->specGlossMap)
        gk__texFlag(specGloss->specGlossMap, "SPECGLOSS", flg);

      break;
    }
    default:
      break;
  }

  SH_VF_ARG("TEX_COUNT %d", flg->texCount)

  /* shadow flags */
  if (GK_FLG(scene->flags, GK_SCENEF_SHADOWS)) {
    int shadSplit;

    SH_VF("SHADOWMAP");

    shadSplit = gkShadowSplit();
    switch (gkShadowTechn()) {
      case GK_SHADOW_CSM:
        SH_VF("SHAD_SPLIT")
        SH_VF("POS_MS")
        SH_VF_ARG("SHAD_SPLIT %d", shadSplit)
        break;
      default:
        break;
    }

    if (light) {
      if (light->type == GK_LIGHT_TYPE_POINT) {
        SH_VF("SHAD_CUBE")
        SH_VF("POS_WS")
      }
    }
  }

  /* transpareny flags */
  if (gkIsTransparent(scene, mat)) {
    if (mat->technique->transparent->color) {
      gk__colorOrTexFlag(mat->technique->transparent->color, "TRANSP", flg);
    } else {
      SH_VF("TRANSP_NO_COLOR")
    }

    switch (mat->technique->transparent->opaque) {
      case GK_OPAQUE_A_ONE:
        SH_VF("TRANSP_A_ONE")
        break;
      case GK_OPAQUE_A_ZERO:
        SH_VF("TRANSP_A_ZERO")
        break;
      case GK_OPAQUE_RGB_ONE:
        SH_VF("TRANSP_RGB_ONE")
        break;
      case GK_OPAQUE_RGB_ZERO:
        SH_VF("TRANSP_RGB_ZERO")
        break;
      default:
        break;
    }

    SH_VF("TRANSP")

    switch (gkTranspTechn()) {
      case GK_TRANSP_WEIGHTED_BLENDED:
        SH_VF("TRANSP_WBL")
        break;
      default:
        break;
    }
  }
}

GkShader*
gkShadersFor(GkScene     * __restrict scene,
             GkLight     * __restrict light,
             GkPrimitive * __restrict prim,
             GkMaterial  * __restrict mat) {
  GkShader    *vert, *frag;
  char        *fragSource[3], *vertSource[3];

  /* TODO: create dynamic by platform */
  vertSource[0] = fragSource[0] = "\n#version 410 \n";

  switch (mat->technique->type) {
    case GK_MATERIAL_PHONG:
      fragSource[2] =
#include "glsl/frag/phong.glsl"
      ;
      break;
    case GK_MATERIAL_BLINN:
      fragSource[2] =
#include "glsl/frag/blinn.glsl"
      ;
      break;
    case GK_MATERIAL_LAMBERT:
      fragSource[2] =
#include "glsl/frag/lambert.glsl"
      ;
      break;
    case GK_MATERIAL_CONSTANT:
      fragSource[2] =
#include "glsl/frag/constant.glsl"
      ;
      break;
    case GK_MATERIAL_METALROUGH:
      fragSource[2] =
#include "glsl/frag/pbr_metalrough.glsl"
      ;
      break;
    case GK_MATERIAL_SPECGLOSS:
      fragSource[2] =
#include "glsl/frag/pbr_specgloss.glsl"
      ;
      break;
    default:
      return NULL;
  }

  gkShaderFlagsFor(scene,
                   light,
                   prim,
                   mat,
                   &vertSource[1],
                   &fragSource[1]);

  vert = calloc(1, sizeof(*vert));
  vert->isValid    = 1;
  vert->shaderType = GL_VERTEX_SHADER;

  vertSource[2] =
#include "glsl/vert/common.glsl"
  ;

  vert->shaderId = gkShaderLoadN(vert->shaderType, vertSource, 3);

  frag = calloc(1, sizeof(*frag));
  frag->isValid    = 1;
  frag->shaderType = GL_FRAGMENT_SHADER;
  frag->shaderId   = gkShaderLoadN(frag->shaderType, fragSource, 3);

  vert->next = frag;

  /* TODO: free  vertSource and fragSource */
  return vert;
}

GkProgram*
gkGetOrCreatProgForCmnMat(GkScene     * __restrict scene,
                          GkLight     * __restrict light,
                          GkPrimitive * __restrict prim,
                          GkMaterial  * __restrict mat) {
  char  name[64];
  void *userData[4];

  (void)gkShaderNameFor(scene, light, prim, mat, name);

  userData[0] = scene;
  userData[1] = light;
  userData[2] = prim;
  userData[3] = mat;

  return gkGetOrCreatProg(name, gk_creatProgForCmnMat, userData);
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

    glBindAttribLocation(prog->progId, index, inp->name);

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
  GkLight     *light;
  GkMaterial  *mat;

  scene = ((void **)userData)[0];
  light = ((void **)userData)[1];
  prim  = ((void **)userData)[2];
  mat   = ((void **)userData)[3];

  if ((shaders = gkShadersFor(scene, light, prim, mat)))
    return gkMakeProgram(shaders, gk__beforeLinking, prim);

  return NULL;
}

static
void
gk__texFlag(GkTexture     * __restrict tex,
            char          * __restrict attrname,
            GkFlagsStruct * __restrict flags) {
  GkSampler  *sampler;
  const char *coordInpName;

  if (!tex)
    return;

  coordInpName = NULL;
  if ((sampler = tex->sampler))
    coordInpName = sampler->coordInputName;
  if (!coordInpName)
    coordInpName = "TEXCOORD";

  flags->frag += sprintf(flags->frag,
                         "\n#define %s_TEX\n"
                         "\n#define %s_TEXCOORD v%s\n",
                         attrname,
                         attrname,
                         coordInpName);
  flags->texCount++;
}

static
void
gk__colorOrTexFlag(GkColorDesc   * __restrict attr,
                   char          * __restrict attrname,
                   GkFlagsStruct * __restrict flags) {
  switch (attr->method) {
    case GK_COLOR_COLOR:
      flags->frag += sprintf(flags->frag, "\n#define %s_COLOR\n", attrname);
      break;
    case GK_COLOR_TEX:
      gk__texFlag(attr->val, attrname, flags);
      break;
    default: break;
  }
}

static
void
gk__fillAttribs(GkMaterial   * __restrict mat,
                GkColorDesc  * __restrict matAttribs[4],
                GkTechnique  * __restrict techn) {
  matAttribs[0] = techn->diffuse;
  matAttribs[1] = techn->specular;
  matAttribs[2] = techn->ambient;
  matAttribs[3] = techn->emission;
}
