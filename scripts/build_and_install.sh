#!/bin/bash
# build_and_install.sh - Clean build and install for local user
# Usage: ./build_and_install.sh [--clean]
set -euo pipefail

BUILD_DIR="${BUILD_DIR:-build}"
INSTALL_PREFIX="${INSTALL_PREFIX:-$HOME/.local}"
BUILD_TYPE="${BUILD_TYPE:-Release}"

cd "$(dirname "$0")"

if [ "${1:-}" = "--clean" ] && [ -d "$BUILD_DIR" ]; then
    echo "Removing previous build directory..."
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

echo "Configuring with CMake..."
cmake -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DCMAKE_INSTALL_PREFIX="$INSTALL_PREFIX" ..

echo "Building..."
cmake --build . -- -j"$(nproc)"

echo "Installing to $INSTALL_PREFIX ..."
cmake --install .

echo "Build and install complete. Binaries are in $INSTALL_PREFIX/bin if applicable."
