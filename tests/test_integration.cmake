# Integration test for madios with example corpus
execute_process(
  COMMAND ${CMAKE_BINARY_DIR}/madios ${CMAKE_SOURCE_DIR}/test/corpus.txt 0.9 0.01 5 0.65
  RESULT_VARIABLE result
  OUTPUT_VARIABLE output
  ERROR_VARIABLE error
)
if(NOT result EQUAL 0)
  message(FATAL_ERROR "madios failed with error: ${error}")
endif()
file(READ "${CMAKE_SOURCE_DIR}/tests/expected_output.txt" expected)
if(NOT output STREQUAL expected)
  message(FATAL_ERROR "madios output does not match expected output!\n--- Actual ---\n${output}\n--- Expected ---\n${expected}")
endif()
