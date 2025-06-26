#!/bin/bash
set -e

# Always start from the script's directory (project root)
cd "$(dirname "$0")"

# Run variable name fix script before building
if [ -x ./fix_variable_names.sh ]; then
    echo "Running fix_variable_names.sh..."
    ./fix_variable_names.sh
else
    echo "Warning: fix_variable_names.sh not found or not executable. Skipping variable name fixes."
fi

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
