#!/bin/bash
# run_all_tests.sh - Run all test executables in the project
# Usage: ./run_all_tests.sh
set -e

cd "$(dirname "$0")"

# Find all test executables in build/ and run them
TEST_DIR="build"

if [ ! -d "$TEST_DIR" ]; then
    echo "Build directory not found. Please build the project first."
    exit 1
fi

# List of known test executables (add more as needed)
TESTS=(
    "$TEST_DIR/tests_basic"
    "$TEST_DIR/tests_all"
    "$TEST_DIR/test_trimspaces_minimal"
)

EXIT_CODE=0
for test_exec in "${TESTS[@]}"; do
    if [ -x "$test_exec" ]; then
        echo "Running $test_exec..."
        "$test_exec" || EXIT_CODE=$?
    else
        echo "Test executable $test_exec not found or not executable."
    fi
    echo
done

exit $EXIT_CODE
