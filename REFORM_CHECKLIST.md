# Madios Refactor & Modernization Checklist

**Note: As of 2025-06-26, the project migration is nearly complete, but some .cpp and .h files remain in the project root and require cleanup. Phase 2 filesystem tasks are not fully complete.**

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
- [x] Move source/header files to new locations
    - [x] Remove empty/obsolete files (all remaining .cpp/.h files in project root were empty and deleted; real code is in src/ or include/madios/)
- [x] Update `CMakeLists.txt` and build scripts for new layout
- [x] Refactor `#include` directives and paths
- [x] Remove obsolete/duplicate files
- [x] Ensure all tests and builds pass after each move
- [x] Update `README.md` and add `CONTRIBUTING.md` with structure rationale
- [x] Filesystem restructuring: Move all public headers to include/madios/, update CMake, and validate build
    - [x] Move headers and test data
    - [x] Update CMakeLists.txt and README
    - [x] Fix all include paths and header dependencies (TNT/Array2D, iostream, etc.)
    - [x] Synchronize TNT/Array2D and related TNT headers from `src/maths/tnt/` to `include/madios/maths/tnt/` (public headers complete)
    - [x] Validate build and tests after restructuring
    - [x] Update all code to use fully qualified TNT types (e.g., `TNT::Array2D`, `TNT::Array1D`) globally
    - [x] Validate build and tests after namespace fix
- [x] Upgrade project to C++14 for modern smart pointer and STL usage
- [x] Migrate all RDSNode and related code/tests to use std::unique_ptr and std::make_unique
- [x] Build and test after smart pointer migration
- [x] Fix all pointer casts and construction in tests and codebase
- [x] Fix variable name and syntax errors in RDSGraph.cpp
- [x] Confirm all tests pass after migration

## Phase 3: Documentation Improvement
- [ ] Add/improve Doxygen-style comments for all public classes/methods
- [ ] Document all parameters, return values, and exceptions
- [ ] Expand `README.md` with usage, install, and CLI instructions
- [ ] Add `docs/` directory for guides, design notes, and algorithm explanations
- [ ] Document build system, test suite, and contribution workflow
- [ ] Add architecture diagrams/flowcharts as needed

## Phase 4: Code Logic Refactor and Modernization
- [x] Plan and document first refactor target (RDSNode smart pointer migration)
- [x] Refactor in small, tested steps (RDSNode and related code)
- [x] Replace raw pointers with smart pointers where appropriate (RDSNode, RDSGraph, tests)
- [x] Use `auto`, range-based for loops, STL algorithms [NEXT]
- [ ] Remove unnecessary `using namespace` and global variables
- [x] Apply `const` correctness and immutability
  - [x] Add `const` correctness to member functions and variables in `RDSGraph.cpp` where possible.  
    - Rationale: Marking functions and variables as `const` when they do not modify the object’s state helps prevent accidental changes, improves code clarity, enables compiler optimizations, and communicates intent to other programmers.  
    - Steps: Review all member functions and mark as `const` if they do not modify the object. Mark local variables as `const` where appropriate. Build and test after changes. Update documentation.  
    - Status: Complete. Build and all tests passed after changes.
- [ ] Replace `assert`/manual checks with exceptions or error codes [NEXT]
    - Rationale: Exceptions and error codes provide safer, more user-friendly error handling than asserts, especially in public APIs. This helps users and contributors understand and recover from errors.
    - Steps: Review all public methods, replace assert with exceptions or error codes, add clear error messages, update tests as needed.
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
- [x] Deep copy/clone and simulation logic for RDSGraph is robust and tested (2025-06-26)
- [ ] Update this checklist and the reform plan as tasks are completed or requirements change
- [ ] Use issue tracker or project board for detailed tracking if desired

---

*Checklist updated after C++14 upgrade and smart pointer migration (2025-06-26). Next: range-based for, auto, const, Doxygen, STL, error handling.*
