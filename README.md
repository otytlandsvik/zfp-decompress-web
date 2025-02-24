# ZFP Decompression compiled for web with WASM

A simple dcompression function for 1-dimensional floating point arrays is exposed.
The original length of the float array must be known.

> [!NOTE] This project is limited to a specific application: compressing a float array on the backend, and decompressing it on the web.

### Build

Docker is required to build the project.
Run `docker_build.sh`, and the library will be available in `dist/`.
