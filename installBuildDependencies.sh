#! /usr/bin/env bash

set -e

CURRENT_DIR="$(dirname $(pwd)/$0)"

function install_musl()
{
    DEPS_DIR="${CURRENT_DIR}/deps"
    mkdir -p "${DEPS_DIR}"
    pushd "${DEPS_DIR}" > /dev/null
        rm -rf ./musl
        git clone --depth=1 git://git.musl-libc.org/musl
        pushd musl > /dev/null
            ./configure \
                --prefix="${DEPS_DIR}/install" \
                --exec-prefix="${DEPS_DIR}" \
                --disable-shared
            make
            make install
        popd > /dev/null
    popd > /dev/null
}

sudo apt-get update
sudo apt-get install -y \
    cmake \
    ninja-build

install_musl

