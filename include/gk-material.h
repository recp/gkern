/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_material_h
#define gk_material_h

typedef enum GkMaterialType {
  GK_MATERIAL_PHONG
} GkMaterialType;

typedef struct GkColorOrTex {
  GkColor color;
} GkColorOrTex;

typedef struct GkMatrialBase {
  GkMaterialType       type;
  uint32_t             index;
  struct GkMatrialBase *next;
} GkMatrialBase;

typedef struct GkPhong {
  GkMatrialBase base;
  GkColorOrTex  emission;
  GkColorOrTex  ambient;
  GkColorOrTex  diffuse;
  GkColorOrTex  specular;
  GkColorOrTex  reflective;
  GkColorOrTex  transparent;
  float         shininess;
  float         reflectivity;
  float         transparency;
  float         indexOfRefraction;
} GkPhong;

typedef struct GkMaterial {
  GkMatrialBase *technique;
  uint8_t        isvalid;
  uint8_t        enabled;
} GkMaterial;

#endif /* gk_material_h */
