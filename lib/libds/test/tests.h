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

#ifndef tests_h
#define tests_h

#include "include/common.h"

/*
 * To register a test:
 *   1. use TEST_DECLARE() to forward declare test
 *   2. use TEST_ENTRY() to add test to list
 */

/* red black tree */
TEST_DECLARE(rb_topdown_str)
TEST_DECLARE(rb_topdown_ptr)
TEST_DECLARE(rb_topdown_custom_cmp)
TEST_DECLARE(rb_topdown_freeenode)
TEST_DECLARE(rb_topdown_custom_cmp_i32)
TEST_DECLARE(rb_topdown_custom_cmp_i64)

/* forward list */
TEST_DECLARE(flist)
TEST_DECLARE(flist_sep)

/* hash table */
TEST_DECLARE(htable)

/*****************************************************************************/

TEST_LIST {
  /* red black tree */
  TEST_ENTRY(rb_topdown_str)
  TEST_ENTRY(rb_topdown_ptr)
  TEST_ENTRY(rb_topdown_custom_cmp)
  TEST_ENTRY(rb_topdown_freeenode)
  TEST_ENTRY(rb_topdown_custom_cmp_i32)
  TEST_ENTRY(rb_topdown_custom_cmp_i64)

  /* forward list */
  TEST_ENTRY(flist)
  TEST_ENTRY(flist_sep)

  /* hash table */
  TEST_ENTRY(htable)
};

#endif /* tests_h */
