/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "time.h"
#include "../common.h"
#include "../../include/gk/gk.h"
#include "../../include/gk/time.h"
#include <time.h>

#ifdef __APPLE__
#  include <mach/mach_time.h>
#endif

#include <stdint.h>

double gk__time2sec = 1.0;

GK_EXPORT
double
gkGetTime() {
#ifdef __APPLE__
  uint64_t abstime;
  abstime = mach_absolute_time();
  return abstime * gk__time2sec;
#elif defined(__WIN32__)
  uint64_t abstime;
  QueryPerformanceCounter((LARGE_INTEGER *)&abstime);
  return abstime * gk__time2sec
#else
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (double)ts.tv_sec + ts.tv_nsec * 1e-9;
#endif
}

void
gk_time_init(void) {
#ifdef __APPLE__
  mach_timebase_info_data_t info;
  mach_timebase_info(&info);
  gk__time2sec = (double)info.numer / (info.denom * NSEC_PER_SEC);
#elif defined(__WIN32__)
  uint64_t frequency;
  QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
  gk__time2sec = 1.0 / (double)frequency;
#endif
}

void
gk_time_deinit(void) { }
