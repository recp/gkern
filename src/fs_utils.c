/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
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

