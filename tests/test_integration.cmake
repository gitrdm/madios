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
# Write madios output to a temporary file for JSON comparison
file(WRITE "${CMAKE_BINARY_DIR}/actual_output.json" "${output}")
execute_process(
  COMMAND python3 "${CMAKE_SOURCE_DIR}/tests/compare_json.py" "${CMAKE_BINARY_DIR}/actual_output.json" "${CMAKE_SOURCE_DIR}/tests/expected_output.json"
  RESULT_VARIABLE json_result
)
if(NOT json_result EQUAL 0)
  file(READ "${CMAKE_BINARY_DIR}/actual_output.json" actual_json)
  file(READ "${CMAKE_SOURCE_DIR}/tests/expected_output.json" expected_json)
  message(FATAL_ERROR "madios JSON output does not match expected output!\n--- Actual ---\n${actual_json}\n--- Expected ---\n${expected_json}")
endif()
