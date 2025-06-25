cmake_minimum_required(VERSION 3.10)
project(madios)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source and header files
file(GLOB SRC_FILES
    "src/*.cpp"
    "src/utils/*.cpp"
    "src/maths/*.cpp"
)
file(GLOB HEADER_FILES
    "src/*.h"
    "src/utils/*.h"
    "src/maths/*.h"
)

# Include directories
include_directories(
    src
    src/utils
    src/maths
    src/maths/tnt
    ${CMAKE_SOURCE_DIR}/include
)

add_executable(madios ${SRC_FILES} ${HEADER_FILES})

# Set output directory to standard CMake build location (build/)
set_target_properties(madios PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

# --- Testing ---
# Enable CTest for modern test integration
enable_testing()

# Add tests_basic as a test executable
add_executable(tests_basic
    tests/test_basic.cpp
    tests/test_core.cpp
    tests/test_equiv.cpp
    tests/test_input_plain.cpp
    tests/test_pcfg_output.cpp
    tests/test_rdsgraph_json.cpp
    tests/test_special.cpp
    tests/test_utils.cpp
    src/BasicSymbol.cpp
    src/EquivalenceClass.cpp
    src/RDSGraph.cpp
    src/RDSNode.cpp
    src/SearchPath.cpp
    src/SignificantPattern.cpp
    src/SpecialLexicons.cpp
    src/logging.cpp
    src/utils/MiscUtils.cpp
    src/utils/Stringable.cpp
    src/utils/TimeFuncs.cpp
    src/maths/special.cpp
)

# Link libraries if needed (e.g., pthread)
target_link_libraries(tests_basic PRIVATE pthread)

# Register with CTest
add_test(NAME tests_basic COMMAND tests_basic)
add_test(NAME integration_test
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/tests/test_integration.cmake
)

# Install rule for the madios binary (user-local install)
install(TARGETS madios DESTINATION bin)
