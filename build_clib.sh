#!/usr/bin/env bash


gcc \
  -shared \
  -o dist/zfp_comp.so -fPIC src/comp.c \
  -O3 `# compile with all optimizations enabled` \
  -flto `# enable link-time optimization` \
  -I /zfp/include -lm `# add the zfp include directory` \
  /zfp/build/lib/libzfp.a

