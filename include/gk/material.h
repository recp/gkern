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
struct GkPipeline;
struct GkPrimitive;
struct GkPrimInst;

typedef enum GkOpaque {
  GK_OPAQUE_OPAQUE   = 0, /* Default */
  GK_OPAQUE_A_ONE    = 1, /* Default */
  GK_OPAQUE_A_ZERO   = 2,
  GK_OPAQUE_RGB_ONE  = 3,
  GK_OPAQUE_RGB_ZERO = 4,
  GK_OPAQUE_BLEND    = 5,
  GK_OPAQUE_MASK     = 6,
  GK_OPAQUE_DEFAULT  = GK_OPAQUE_A_ONE
} GkOpaque;

typedef enum GkMaterialType {
  GK_MATERIAL_PHONG      = 1,
  GK_MATERIAL_BLINN      = 2,
  GK_MATERIAL_LAMBERT    = 3,
  GK_MATERIAL_CONSTANT   = 4,
  GK_MATERIAL_METALROUGH = 5,
  GK_MATERIAL_SPECGLOSS  = 6
} GkMaterialType;

typedef enum GkColorMethod {
  GK_COLOR_DISCARD = 0,
  GK_COLOR_TEX     = 1,
  GK_COLOR_COLOR   = 2
} GkColorMethod;

typedef struct GkColorDesc {
  void         *val;
  GkColor      *color;
  GkTexture    *tex;
  GkColorMethod method;
} GkColorDesc;

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

typedef struct GkOcclusion {
  GkTexture *tex;
  float      strength;
} GkOcclusion;

typedef struct GkNormalMap {
  GkTexture *tex;
  float      scale;
} GkNormalMap;

typedef struct GkTechnique {
  struct GkTechnique *next;

  GkTransparent      *transparent;
  GkReflective       *reflective;
  GkColorDesc        *diffuse;
  GkColorDesc        *specular;
  GkColorDesc        *emission;
  GkColorDesc        *ambient;
  GkOcclusion        *occlusion;
  GkNormalMap        *normal;

  struct GkPass      *pass;
  GkMaterialType      type;
  float               shininess;
} GkTechnique;

/* Common PBR Materials */

typedef struct GkMetalRough {
  GkTechnique base;
  GkColor     albedo;
  GkTexture  *albedoMap;
  GkTexture  *metalRoughMap;
  float       metallic;
  float       roughness;
} GkMetalRough;

typedef struct GkSpecGloss {
  GkTechnique base;
  GkColor     diffuse;
  GkColor     specular;
  GkTexture  *diffuseMap;
  GkTexture  *specGlossMap;
  float       gloss;
} GkSpecGloss;

typedef struct GkBindTexture {
  struct GkBindTexture *next;
  GkTexture            *texture;
  const char           *coordInputName;
} GkBindTexture;

typedef struct GkMaterial {
  GkTechnique   *technique;
  FListItem     *boundTextures;
  float          indexOfRefraction;
  bool           doubleSided;
  uint8_t        isvalid;
  uint8_t        enabled;
} GkMaterial;

GkTechnique*
gkMaterialNewPhong(void);

GkTechnique*
gkMaterialNewBlinn(void);

GkTechnique*
gkMaterialNewLambert(void);

GkTechnique*
gkMaterialNewConstant(void);

GkMetalRough*
gkMaterialNewMetalRough(void);

GkSpecGloss*
gkMaterialNewSpecGloss(void);

void
gkUniformMaterial(struct GkContext  * __restrict ctx,
                  struct GkPipeline * __restrict prog,
                  struct GkMaterial * __restrict material);

void
gkUniformMaterialStruct(struct GkContext  * __restrict ctx,
                        struct GkPipeline * __restrict prog,
                        struct GkMaterial * __restrict material);

GkMaterial*
gkMaterialFor(struct GkScene     *scene,
              struct GkModelInst *modelInst,
              struct GkPrimInst  *primInst);

#endif /* gk_material_h */
