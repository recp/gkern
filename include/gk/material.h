/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_material_h
#define gk_material_h

#include "common.h"
#include "color.h"
#include "texture.h"

struct GkPass;
struct GkScene;
struct GkModelInst;
struct GkProgram;
struct GkPrimitive;
struct GkPrimInst;

typedef enum GkOpaque {
  GK_OPAQUE_A_ONE    = 0, /* Default */
  GK_OPAQUE_A_ZERO   = 1,
  GK_OPAQUE_RGB_ONE  = 2,
  GK_OPAQUE_RGB_ZERO = 3,
  GK_OPAQUE_DEFAULT  = GK_OPAQUE_A_ONE
} GkOpaque;

typedef enum GkMaterialType {
  GK_MATERIAL_PHONG      = 1,
  GK_MATERIAL_BLINN      = 2,
  GK_MATERIAL_LAMBERT    = 3,
  GK_MATERIAL_CONSTANT   = 4,
  GK_MATERIAL_METALROUGH = 5
} GkMaterialType;

typedef enum GkColorMethod {
  GK_COLOR_DISCARD = 0,
  GK_COLOR_TEX     = 1,
  GK_COLOR_COLOR   = 2
} GkColorMethod;

typedef struct GkColorDesc {
  void         *val;
  GkColorMethod method;
} GkColorDesc;

typedef struct GkTechnique {
  GkMaterialType      type;
  uint32_t            index; /* subroutine index */
  char               *subroutine;
  struct GkPass      *pass;
  struct GkTechnique *next;
} GkTechnique;

typedef struct GkPhong {
  GkTechnique  base;
  GkColorDesc *emission;
  GkColorDesc *ambient;
  GkColorDesc *diffuse;
  GkColorDesc *specular;
  float        shininess;
} GkPhong;

typedef struct GkLambert {
  GkTechnique  base;
  GkColorDesc *emission;
  GkColorDesc *ambient;
  GkColorDesc *diffuse;
} GkLambert;

typedef struct GkConstant {
  GkTechnique  base;
  GkColorDesc *emission;
} GkConstant;

typedef GkPhong GkBlinn;

/* Common PBR Materials */

typedef struct GkMetalRough {
  GkTechnique base;
  GkColor     albedo;
  GkTexture  *albedoTex;
  GkTexture  *metalRoughTex;
  float       metallic;
  float       roughness;
} GkMetalRough;

typedef enum GkAlphaMode {
  GK_ALPHA_OPAQUE,
  GK_ALPHA_MASK,
  GK_ALPHA_BLEND
} GkAlphaMode;

typedef struct GkTransparent {
  GkColorDesc *color;
  GkAlphaMode  mode;
  GkOpaque     opaque;
  float        amount;
  float        cutoff;
} GkTransparent;

typedef struct GkReflective {
  GkColorDesc *color;
  float        amount;
} GkReflective;

typedef struct GkMaterial {
  GkTechnique   *technique;
  GkTransparent *transparent;
  GkReflective  *reflective;
  FListItem     *boundTextures;
  float          indexOfRefraction;
  uint8_t        isvalid;
  uint8_t        enabled;
} GkMaterial;

GkPhong*
gkMaterialNewPhong(void);

GkBlinn*
gkMaterialNewBlinn(void);

GkLambert*
gkMaterialNewLambert(void);

GkConstant*
gkMaterialNewConstant(void);

GkMetalRough*
gkMaterialNewMetalRough(void);

void
gkUniformMaterial(struct GkContext  * __restrict ctx,
                  struct GkProgram  * __restrict prog,
                  struct GkMaterial * __restrict material);

void
gkUniformMaterialStruct(struct GkContext  * __restrict ctx,
                        struct GkProgram  * __restrict prog,
                        struct GkMaterial * __restrict material);

GkMaterial*
gkMaterialFor(struct GkScene     *scene,
              struct GkModelInst *modelInst,
              struct GkPrimInst  *primInst);

#endif /* gk_material_h */
