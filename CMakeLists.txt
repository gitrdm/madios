cmake_minimum_required(VERSION 3.10)
project(madios)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source and header files
file(GLOB SRC_FILES
    "src/*.cpp"
    "src/utils/*.cpp"
    "src/maths/*.cpp"
    "src/Logger.cpp" # Logging implementation
)
file(GLOB HEADER_FILES
    "include/madios/*.h"
    "include/madios/utils/*.h"
    "include/madios/maths/*.h"
    "include/madios/maths/tnt/*.h"
    "include/madios/Logger.h" # Logging header
)

# Include directories
include_directories(
    include
    include/madios
    include/madios/utils
    include/madios/maths
    include/madios/maths/tnt
    src
    src/utils
    src/maths
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
    src/Logger.cpp # Ensure Logger is linked
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

# Add logger test
add_executable(test_logger tests/test_logger.cpp src/Logger.cpp)
target_include_directories(test_logger PRIVATE include)
target_link_libraries(test_logger PRIVATE pthread)
add_test(NAME test_logger COMMAND test_logger)

# Add a dedicated test for clone path in RDSGraph
target_include_directories(tests_basic PRIVATE include)

add_executable(test_rdsgraph_clone tests/test_rdsgraph_clone.cpp
    src/BasicSymbol.cpp
    src/EquivalenceClass.cpp
    src/RDSGraph.cpp
    src/RDSNode.cpp
    src/SearchPath.cpp
    src/SignificantPattern.cpp
    src/SpecialLexicons.cpp
    src/Logger.cpp
    src/utils/MiscUtils.cpp
    src/utils/Stringable.cpp
    src/utils/TimeFuncs.cpp
    src/maths/special.cpp
)
target_include_directories(test_rdsgraph_clone PRIVATE include)
target_link_libraries(test_rdsgraph_clone PRIVATE pthread)
add_test(NAME test_rdsgraph_clone COMMAND test_rdsgraph_clone)

# Version and git commit macros for logging
execute_process(
  COMMAND git rev-parse --short HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
add_definitions(-DMADIOS_VERSION=\"2.0.0-pre\")
add_definitions(-DMADIOS_GIT_COMMIT=\"${GIT_COMMIT_HASH}\")
# Install rule for the madios binary (user-local install)
install(TARGETS madios DESTINATION bin)

add_library(madioslib
    src/BasicSymbol.cpp
    src/EquivalenceClass.cpp
    src/RDSGraph.cpp
    src/RDSNode.cpp
    src/SearchPath.cpp
    src/SignificantPattern.cpp
    src/SpecialLexicons.cpp
    src/utils/MiscUtils.cpp
    src/utils/Stringable.cpp
    src/utils/TimeFuncs.cpp
    src/maths/special.cpp
)

target_include_directories(madioslib PUBLIC include)
