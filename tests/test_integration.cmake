# Integration test for madios with example corpus (JSON output)
# Ensure CMAKE_BINARY_DIR and CMAKE_SOURCE_DIR are set for cmake -P
set(CMAKE_BINARY_DIR "${CMAKE_CURRENT_LIST_DIR}/../build")
set(CMAKE_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/..")
execute_process(
  COMMAND ${CMAKE_BINARY_DIR}/madios ${CMAKE_SOURCE_DIR}/test/corpus.txt 0.9 0.01 5 0.65 --json
  RESULT_VARIABLE result
  OUTPUT_VARIABLE output
  ERROR_VARIABLE error
)
if(NOT result EQUAL 0)
  message(FATAL_ERROR "madios failed with error: ${error}")
endif()
file(READ "${CMAKE_SOURCE_DIR}/tests/expected_output.json" expected_json)
# Remove timing field from both outputs for comparison
string(REGEX REPLACE "[ \t\r\n]+" "" output "${output}")
string(REGEX REPLACE "[ \t\r\n]+" "" expected_json "${expected_json}")
string(REGEX REPLACE ",[ \t\r\n]*\"timing\"[ \t\r\n]*:[^,}]*" "" output "${output}")
string(REGEX REPLACE ",[ \t\r\n]*\"timing\"[ \t\r\n]*:[^,}]*" "" expected_json "${expected_json}")
if(NOT output STREQUAL expected_json)
  message(FATAL_ERROR "madios JSON output does not match expected output!\n--- Actual ---\n${output}\n--- Expected ---\n${expected_json}")
endif()
