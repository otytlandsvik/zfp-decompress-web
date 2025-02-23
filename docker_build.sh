#!/usr/bin/env bash

set -e  # Exit immediately if any command fails

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd "$SCRIPT_DIR"

# Ensure a build type is provided
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 {web|clib}"
    exit 1
fi

BUILD_TYPE="$1"

# Determine Dockerfile and output directory based on argument
case "$BUILD_TYPE" in
    web)
        DOCKERFILE="web.Dockerfile"
        OUTPUT_DIR="dist/web"
        ;;
    clib)
        DOCKERFILE="clib.Dockerfile"
        OUTPUT_DIR="dist/clib"
        ;;
    *)
        echo "Invalid argument: $BUILD_TYPE"
        echo "Usage: $0 {web|clib}"
        exit 1
        ;;
esac

# Build the appropriate Docker image
docker build -f "$DOCKERFILE" . -t "$BUILD_TYPE-zfp"

# Ensure the output directory exists
mkdir -p "$OUTPUT_DIR"

# Run the appropriate build process inside the container
docker run --rm -v "${SCRIPT_DIR}/${OUTPUT_DIR}":/${BUILD_TYPE}-zfp/dist -t "$BUILD_TYPE-zfp" ./build_${BUILD_TYPE}.sh
