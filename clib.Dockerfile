FROM ubuntu:22.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    cmake \
    g++ \
    make \
    ninja-build \
    git \
    && rm -rf /var/lib/apt/lists/*

# Clone the ZFP repository at a specific tag
RUN mkdir -p /zfp && \
    cd /zfp && \
    git init && \
    git remote add origin https://github.com/LLNL/zfp.git && \
    git fetch --depth 1 origin release1.0.1 && \
    git checkout FETCH_HEAD

# Build the library
RUN mkdir -p /zfp/build && \
    cd /zfp/build && \
    cmake -DBUILD_SHARED_LIBS=OFF .. && \
    make

WORKDIR /clib-zfp
COPY build_clib.sh .
COPY src/clib ./src
