#!/usr/bin/env bash


gcc \
  /zfp/build/lib/libzfp.so -o dist/zfp_comp.so src/comp.c \
  -O3 `# compile with all optimizations enabled` \
  -flto `# enable link-time optimization` \
  -I /zfp/include `# add the zfp include directory` \

