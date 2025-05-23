#include "zfp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* test f# interop */
void hello_world() { printf("Hello, from clib!\n"); }

/* compress array */
char *compress(float *array, size_t nx, double tolerance, size_t *out_size) {
  zfp_type type;     /* array scalar type */
  zfp_field *field;  /* array meta data */
  zfp_stream *zfp;   /* compressed stream */
  void *buffer;      /* storage for compressed stream */
  size_t bufsize;    /* byte size of compressed buffer */
  bitstream *stream; /* bit stream to write to or read from */

  /* allocate meta data for the 1D array a[nx] */
  type = zfp_type_float;
  field = zfp_field_1d(array, type, nx);

  /* allocate meta data for a compressed stream */
  zfp = zfp_stream_open(NULL);

  /* set compression mode and parameters via one of four functions */
  /*  zfp_stream_set_reversible(zfp); */
  /*  zfp_stream_set_rate(zfp, rate, type, zfp_field_dimensionality(field),
   * zfp_false); */
  /*  zfp_stream_set_precision(zfp, precision); */
  zfp_stream_set_accuracy(zfp, tolerance);

  /* allocate buffer for compressed data */
  bufsize = zfp_stream_maximum_size(zfp, field);
  buffer = malloc(bufsize);

  /* associate bit stream with allocated buffer */
  stream = stream_open(buffer, bufsize);
  zfp_stream_set_bit_stream(zfp, stream);
  zfp_stream_rewind(zfp);

  /* compress array and output compressed stream */
  *out_size = zfp_compress(zfp, field);
  if (!(*out_size)) {
    fprintf(stderr, "compression failed\n");
    return NULL;
  }

  /* clean up */
  zfp_field_free(field);
  zfp_stream_close(zfp);
  stream_close(stream);
  // free(buffer);
  // free(array);

  return buffer;
}

/* Compress array, writing results to a given file */
int compress_to_file(float *array, size_t nx, double tolerance,
                     char *out_path) {
  size_t out_size;

  /* Compress array */
  char *compressed = compress(array, nx, tolerance, &out_size);
  if (compressed == NULL)
    return 1;
  if (out_size <= 0) {
    free(compressed);
    return 2;
  }

  /* Open outfile for writing */
  FILE *fp = fopen(out_path, "wb");
  if (fp == NULL) {
    free(compressed);
    return 3;
  }

  /* Write compressed buffer to file */
  size_t written_bytes = fwrite(compressed, 1, out_size, fp);
  if (written_bytes != out_size) {
    free(compressed);
    return 4;
  }

  /* Clean up */
  fclose(fp);
  free(compressed);

  return 0;
}
