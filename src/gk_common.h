/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_src_common_h
#define gk_src_common_h

#define GK_INTERN

#ifdef _MSC_VER
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>

#  define strncasecmp _strnicmp
#  define strcasecmp  _stricmp
#endif

#ifdef __GNUC__
#  define GK_DESTRUCTOR __attribute__((destructor))
#  define GK_CONSTRUCTOR __attribute__((constructor))
#else
#  define GK_DESTRUCTOR
#  define GK_CONSTRUCTOR
#endif

#define GK__UNUSED(X) (void)X

#endif /* gk_src_common_h */
