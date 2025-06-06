#!/bin/bash
# build_and_install.sh - Clean build and install for local user
# Usage: ./build_and_install.sh
set -e

# Go to project root (directory of this script)
cd "$(dirname "$0")"

# Remove previous build directory if it exists
if [ -d build ]; then
    echo "Removing previous build directory..."
    rm -rf build
fi

# Create and enter build directory
mkdir build
cd build

# Configure with CMake (install to user local prefix)
cmake -DCMAKE_INSTALL_PREFIX="$HOME/.local" ..

# Build all targets
make -j$(nproc)

# Install to $HOME/.local
make install

echo "Build and install complete. Binaries are in $HOME/.local/bin if applicable."
