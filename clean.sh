#! /usr/bin/env bash

set -e

BUILD_DIR="$(dirname $0)/build"
rm -rf ${BUILD_DIR:?}
