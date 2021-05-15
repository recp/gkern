/*
 * This file is part of the gk project (https://github.com/recp/gk)
 * Copyright (c) Recep Aslantas.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "include/common.h"
#include "tests.h"

#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TARGET_NAME "libds"

int
main(int argc, const char * argv[]) {
  test_entry_t *entry;
  test_status_t st;
  int32_t       i, count, passed, failed, maxlen;
  double        start, end, elapsed, total;

  passed = failed = maxlen  = 0;
  total  = 0.0;
  count  = sizeof(tests) / sizeof(tests[0]);

  fprintf(stderr, CYAN "\nWelcome to %s tests\n\n" RESET, TARGET_NAME);

  for (i = 0; i < count; i++) {
    int32_t len;

    entry = tests + i;
    len   = (int32_t)strlen(entry->name);

    if (len > maxlen)
      maxlen = len;
  }

  maxlen += 5;

  fprintf(stderr,
          BOLDWHITE  "  %-*s    %-*s\n",
          maxlen, "Test Name", maxlen, "Elapsed Time");

  for (i = 0; i < count; i++) {
    entry   = tests + i;
    start   = clock();
    st      = entry->entry();
    end     = clock();
    elapsed = (end - start) / CLOCKS_PER_SEC;
    total  += elapsed;

    if (!st.status) {
      fprintf(stderr,
              BOLDRED  "  " FAIL_TEXT BOLDWHITE " %s " RESET, entry->name);
      if (st.msg) {
        fprintf(stderr,
                YELLOW "- %s" RESET,
                st.msg);
      }

      fprintf(stderr, "\n");

      failed++;
    } else {
      fprintf(stderr, GREEN  "  " OK_TEXT RESET " %-*s  ", maxlen, entry->name);

      if (elapsed > 0.01)
        fprintf(stderr, YELLOW "%.2fs", elapsed);
      else
        fprintf(stderr, "0");

      fprintf(stderr, "\n" RESET);
      passed++;
    }
  }

  if (failed == 0) {
    fprintf(stderr,
            BOLDGREEN "\n  All tests are passed " FINAL_TEXT "\n" RESET);
  }

  fprintf(stderr,
          CYAN "\n%s test results (%0.2fs):\n" RESET
          "--------------------------\n"

          MAGENTA "%d" RESET " tests are runned, "
          GREEN   "%d" RESET " %s passed, "
          RED     "%d" RESET " %s failed\n\n" RESET,
          TARGET_NAME,
          total,
          count,
          passed,
          passed > 1 ? "are" : "is",
          failed,
          failed > 1 ? "are" : "is");

  return failed;
}
