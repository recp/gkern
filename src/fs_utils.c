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

#include "common.h"
#include "fs_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int
fsReadfile(const char * __restrict file,
           const char * __restrict modes,
           char ** __restrict dest) {
  FILE * infile = fopen(file, modes);
  int infile_no = fileno(infile);

  struct stat infile_st;
  if (fstat(infile_no, &infile_st) != 0)
    goto err;

  blksize_t blksize = infile_st.st_blksize;
  off_t fsize = infile_st.st_size;

  size_t fcontents_size = sizeof(char) * fsize;
  *dest = malloc(fcontents_size + 1);
  memset(*dest + fcontents_size, '\0', 1);

  size_t total_read = 0;
  size_t nread = 0;
  do {
    nread = fread(*dest + total_read,
                  sizeof(**dest),
                  blksize,
                  infile);

    total_read += nread;
  } while (nread > 0 && total_read < fsize);

  fclose(infile);

  return 0;
err:
  return -1;
}

