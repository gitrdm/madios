# Madios Refactor & Modernization Checklist

Use this checklist to track progress through the reform plan. Check off each item as you complete it. Update as needed.

---

## Phase 1: Robust Regression Test Suite
- [x] Audit all existing tests in `tests/` directory
- [x] Identify and document test coverage gaps
- [x] Add unit tests for uncovered logic (e.g., `RDSGraph`, `EquivalenceClass`, `SignificantPattern`)
- [x] Add integration tests for CLI, output formats, and error conditions
- [x] Ensure tests cover edge cases and invalid input
- [x] Integrate all tests with CTest
- [x] Ensure tests run in CI (if available)
- [x] Add/verify output comparison scripts (`compare_json.py`, `diff_grammar.py`)
- [x] Create `TESTING.md` with test suite instructions
- [x] Add tests for error handling and invalid input (malformed corpus, bad CLI args)
  - [x] Error handling: malformed corpus input (`test_error_handling_malformed_corpus.cpp`)
  - [x] Error handling: bad CLI arguments (`test_error_handling_bad_cli.cpp`)
- [x] Add tests for exception safety and memory errors
  - [x] Exception safety: RDSGraph public API (`test_exception_safety_rdsgraph.cpp`)
- [x] Add tests for all public methods in new/utility code
  - [x] Public API coverage: MiscUtils (`test_utils_api_coverage.cpp`)
- [x] (Optional) Add performance regression tests
  - [x] Performance regression: RDSGraph (`test_performance_rdsgraph.cpp`)
- [x] Confirm PCFG output is normalized to probabilities and matches regression baseline

## Logging/Tracing Infrastructure (Pre-Phase 2)
- [x] Design and implement minimal logging/tracing utility (header + implementation)
- [x] Integrate logging into key modules and entry points
- [x] Document logging usage in README and code comments
- [x] Use logging to verify correctness during and after refactor
- [x] Capture and save known good baseline logs from CLI and test runs
- [x] Document baseline log comparison process in TESTING.md
- [x] Add trace/info logging at major logic intersections (function entry/exit, key branches, data mutations)
  - [x] Add trace/info logging at:
    - All major public method entry/exit in core classes (RDSGraph, EquivalenceClass, SignificantPattern, ParseTree, etc.) [IN PROGRESS]
    - Key data mutations (nodes/edges/patterns add/remove/rewire)
    - Important branches and error/exception handling
    - Input/output operations (file open/read/write, CLI start/end)
    - Performance-critical sections (optional timing)
    - Integration points with utility modules
- [x] Capture and review enhanced logs for new baseline
- [x] Log madios version, git commit hash, and build date at CLI start
- [x] Log full CLI command/arguments used for the run
- [x] Log input and output file names/paths
- [x] Log deterministic random seed for reproducibility
- [x] Log summary statistics at end (patterns found, rewiring ops, final graph size, etc.)
- [x] Log all error/warning events
- [x] All staged code and logs committed and pushed (2025-06-25)
- [ ] Add more granular trace logs for data mutations and branch decisions during refactor [CONTINUING]

## Baseline Log and Script Infrastructure
- [x] Create `logs/` directory for baseline and regression logs
- [x] Create `scripts/` directory for automation and comparison scripts
- [x] Ensure baseline log (`logs/baseline_run.log`) is generated and versioned
- [x] Document baseline log and output generation process in `TESTING.md`
- [x] Add instructions for using scripts to compare logs and outputs
- [x] Review and update baseline after major changes

## Phase 2: Filesystem and Project Structure Refactor
- [x] Propose and document new directory structure (see REFORM_PLAN.md and README)
- [ ] Move source/header files to new locations
- [ ] Update `CMakeLists.txt` and build scripts for new layout
- [ ] Refactor `#include` directives and paths
- [ ] Remove obsolete/duplicate files
- [ ] Ensure all tests and builds pass after each move
- [ ] Update `README.md` and add `CONTRIBUTING.md` with structure rationale
- [ ] Filesystem restructuring: Move all public headers to include/madios/, update CMake, and validate build
    - [x] Move headers and test data
    - [x] Update CMakeLists.txt and README
    - [x] Fix all include paths and header dependencies (TNT/Array2D, iostream, etc.)
    - [x] Synchronize TNT/Array2D and related TNT headers from `src/maths/tnt/` to `include/madios/maths/tnt/` (public headers complete)
    - [x] Validate build and tests after restructuring
    - [x] Update all code to use fully qualified TNT types (e.g., `TNT::Array2D`, `TNT::Array1D`) globally
    - [x] Validate build and tests after namespace fix

## Phase 3: Documentation Improvement
- [ ] Add/improve Doxygen-style comments for all public classes/methods
- [ ] Document all parameters, return values, and exceptions
- [ ] Expand `README.md` with usage, install, and CLI instructions
- [ ] Add `docs/` directory for guides, design notes, and algorithm explanations
- [ ] Document build system, test suite, and contribution workflow
- [ ] Add architecture diagrams/flowcharts as needed

## Phase 4: Code Logic Refactor and Modernization
- [ ] Plan and document first refactor target (e.g., `RDSGraph`)
- [ ] Refactor in small, tested steps (one module/class at a time)
- [ ] Replace raw pointers with smart pointers where appropriate
- [ ] Use `auto`, range-based for loops, STL algorithms
- [ ] Remove unnecessary `using namespace` and global variables
- [ ] Apply `const` correctness and immutability
- [ ] Replace `assert`/manual checks with exceptions or error codes
- [ ] Add robust input validation and error messages
- [ ] Profile and optimize hot paths if needed
- [ ] Avoid unnecessary copies/allocations
- [ ] Apply consistent code style (`clang-format`)
- [ ] Add `.clang-format` file
- [ ] Remove unused functions, classes, and files
- [ ] Mark deprecated APIs and provide migration paths if needed

## Phase 5: Final Review and Release
- [ ] Run all tests and verify 100% pass rate
- [ ] Proofread and finalize all documentation
- [ ] Conduct thorough code review (self or peer)
- [ ] Tag repository for new release (e.g., `v2.0.0`)
- [ ] Announce changes in `CHANGELOG.md`

---

## Ongoing
- [ ] Update this checklist and the reform plan as tasks are completed or requirements change
- [ ] Use issue tracker or project board for detailed tracking if desired

---

*Keep this checklist up to date as you progress through the refactor and modernization process.*
