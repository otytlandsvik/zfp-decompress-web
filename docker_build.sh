#!/usr/bin/env bash

set -e  # Exit immediately if any command fails

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd "$SCRIPT_DIR"

# Function to build a specific target
docker_build_and_run() {
    local BUILD_TYPE="$1"
    local DOCKERFILE="${BUILD_TYPE}.Dockerfile"
    local OUTPUT_DIR="dist/${BUILD_TYPE}"
    local IMAGE_TAG="${BUILD_TYPE}-zfp"

    echo "Building $BUILD_TYPE..."
    docker build -f "$DOCKERFILE" . -t "$IMAGE_TAG"
    mkdir -p "$OUTPUT_DIR"
    docker run --rm -v "${SCRIPT_DIR}/${OUTPUT_DIR}":/${BUILD_TYPE}-zfp/dist -t "${IMAGE_TAG}" ./build_${BUILD_TYPE}.sh
}

# Determine build types
if [ "$#" -eq 0 ]; then
    docker_build_and_run "web"
    docker_build_and_run "clib"
elif [ "$#" -eq 1 ]; then
    case "$1" in
        web)
            docker_build_and_run "web"
            ;;
        clib)
            docker_build_and_run "clib"
            ;;
        *)
            echo "Invalid argument: $1"
            echo "Usage: $0 {web|clib}"
            exit 1
            ;;
    esac
else
    echo "Usage: $0 {web|clib}"
    exit 1
fi
