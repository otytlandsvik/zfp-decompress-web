#include "zfp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* test f# interop */
void hello_world() { printf("Hello, from clib!\n"); }

/* compress array */
char *compress(double *array, size_t nx, double tolerance, size_t *out_size) {
  zfp_type type;     /* array scalar type */
  zfp_field *field;  /* array meta data */
  zfp_stream *zfp;   /* compressed stream */
  void *buffer;      /* storage for compressed stream */
  size_t bufsize;    /* byte size of compressed buffer */
  bitstream *stream; /* bit stream to write to or read from */

  /* allocate meta data for the 1D array a[nx] */
  type = zfp_type_double;
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
