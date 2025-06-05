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
)

add_executable(madios ${SRC_FILES} ${HEADER_FILES})

# Set output directory to standard CMake build location (build/)
set_target_properties(madios PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
