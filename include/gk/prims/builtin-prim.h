/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef builtin_prim_h
#define builtin_prim_h

#include "../common.h"

struct GkScene;
struct GkPrimitive;

typedef enum GkPrimType {
  GK_PRIM_CUBE    = 1,
  GK_PRIM_TEXQUAD = 2
} GkPrimType;

GK_EXPORT
struct GkPrimitive*
gkGetBuiltinPrim(GkPrimType primType);

GK_EXPORT
struct GkPrimitive*
gkPrimGetTexQuad(void);

GK_EXPORT
struct GkPrimitive*
gkGetBuiltinPrim(GkPrimType primType);

GK_EXPORT
void
gkRenderBuiltinPrim(struct GkScene *scene,
                    GkPrimType      primType);

#endif /* builtin_prim_h */
