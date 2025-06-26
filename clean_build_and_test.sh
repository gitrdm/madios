#!/bin/bash
set -e

# Always start from the script's directory (project root)
cd "$(dirname "$0")"

echo "Cleaning build directory..."
rm -rf build/
mkdir build
cd build

echo "Configuring project with CMake..."
cmake ..

echo "Building project..."
make -j$(nproc)

echo "Running tests..."
ctest --output-on-failure

# Optionally return to project root
cd ..

echo "Clean build and test complete."
