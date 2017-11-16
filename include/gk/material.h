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

typedef enum GkMaterialType {
  GK_MATERIAL_PHONG    = 1,
  GK_MATERIAL_BLINN    = 2,
  GK_MATERIAL_LAMBERT  = 3,
  GK_MATERIAL_CONSTANT = 4
} GkMaterialType;

typedef enum GkColorMethod {
  GK_COLOR_DISCARD = 0,
  GK_COLOR_TEX     = 1,
  GK_COLOR_COLOR   = 2
} GkColorMethod;

typedef struct GkColorOrTex {
  void         *val;
  GkColorMethod method;
} GkColorOrTex;

typedef struct GkTechnique {
  GkMaterialType      type;
  uint32_t            index; /* subroutine index */
  char               *subroutine;
  struct GkPass      *pass;
  struct GkTechnique *next;
} GkTechnique;

typedef struct GkPhong {
  GkTechnique   base;
  GkColorOrTex *emission;
  GkColorOrTex *ambient;
  GkColorOrTex *diffuse;
  GkColorOrTex *specular;
  float         shininess;
} GkPhong;

typedef struct GkLambert {
  GkTechnique   base;
  GkColorOrTex *emission;
  GkColorOrTex *ambient;
  GkColorOrTex *diffuse;
} GkLambert;

typedef struct GkConstant {
  GkTechnique   base;
  GkColorOrTex *emission;
} GkConstant;

typedef GkPhong GkBlinn;

typedef enum GkAlphaMode {
  GK_ALPHA_OPAQUE,
  GK_ALPHA_MASK,
  GK_ALPHA_BLEND
} GkAlphaMode;

typedef struct GkTransparent {
  GkColorOrTex *color;
  GkAlphaMode   mode;
  float         amount;
  float         cutoff;
} GkTransparent;

typedef struct GkReflective {
  GkColorOrTex *color;
  float         amount;
} GkReflective;

typedef struct GkMaterial {
  GkTechnique   *technique;
  GkTransparent *transparent;
  GkReflective  *reflective;
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

void
gkUniformMaterial(struct GkContext  * __restrict ctx,
                  struct GkProgram  * __restrict prog,
                  struct GkMaterial * __restrict material);

void
gkUniformMaterialStruct(struct GkContext  * __restrict ctx,
                        struct GkProgram  * __restrict prog,
                        struct GkMaterial * __restrict material);

#endif /* gk_material_h */
