# Madios Testing Guide

This document describes how to run, extend, and interpret the test suite for the Madios C++ project. It covers both unit and integration/regression tests, as well as output comparison scripts.

---

## Running the Test Suite

### 1. Build All Tests
From the project root, build the project and all tests:

```bash
mkdir -p build
cd build
cmake ..
make -j
```

### 2. Run All Tests with CTest

```bash
cd build
ctest --output-on-failure
```
- This runs all C++ tests (all test files in `tests/` are compiled into a single test executable).
- Output will show which tests pass/fail.

### 3. Run Integration/Regression Tests

#### JSON Output Regression
```bash
cd tests
./test_regression_json.sh
```
- Compares actual output to expected output using `compare_json.py`.

#### PCFG Output Regression
```bash
cd tests
./test_regression_pcfg.sh
```
- Compares actual PCFG output to expected using `compare_pcfg_canonical.py` and `diff_grammar.py`.

---

## Extending the Test Suite

- Add new unit tests in the `tests/` directory as `.cpp` files.
- To add a new test file, update the CMake configuration if you want a separate test executable, or add to the main test target if using the all-in-one approach.
- Add new regression/integration scripts as needed for new output formats or behaviors.

---

## Output Comparison Scripts

- `compare_json.py`: Compares two JSON files for object equality, ignoring whitespace, order, and timing fields. Used for robust regression testing.
- `diff_grammar.py`: Compares grammars as sets of rules, ignoring order and whitespace.
- `compare_pcfg_canonical.py`: Canonicalizes and compares PCFG outputs.

---

## Interpreting Test Results

- All tests should pass before merging or releasing changes.
- If a regression test fails, use the diff scripts to diagnose formatting/content issues.
- If a unit test fails, check the test logic and recent code changes.

---

## CI Integration

- (Optional) Integrate CTest and regression scripts into your CI pipeline for automated testing on every push/PR.

---

### New Test Stubs for Coverage Gaps

The following test stubs have been created to address the identified coverage gaps:

- `tests/test_error_handling_malformed_corpus.cpp`: Tests error handling for malformed corpus input.
- `tests/test_error_handling_bad_cli.cpp`: Tests error handling for bad CLI arguments.
- `tests/test_exception_safety_rdsgraph.cpp`: Tests exception safety for RDSGraph public API.
- `tests/test_utils_api_coverage.cpp`: Tests public API coverage for MiscUtils.
- `tests/test_performance_rdsgraph.cpp`: Stub for performance regression testing of RDSGraph.

These stubs should be implemented and extended as needed to ensure robust regression coverage before proceeding to the next phase of the reform plan.

---

*For questions or to extend the test suite, see the reform plan and checklist for guidance.*
