#! /usr/bin/env bash

set -e

CURRENT_DIR="$(dirname $0)"
BUILD_DIR="${CURRENT_DIR}/build"

mkdir -p ${BUILD_DIR}
pushd ${BUILD_DIR} > /dev/null
    cmake -G "Ninja Multi-Config" ..
    ninja -f build-Debug.ninja
    ninja -f build-Release.ninja
    ninja -f build-RelWithDebInfo.ninja
popd > /dev/null

