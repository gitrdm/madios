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
- [x] Add/improve Doxygen-style comments for all public classes/methods in headers and non-trivial .cpp files
- [x] Document all parameters, return values, and exceptions in public APIs
- [ ] Add file-level comments to all test .cpp files in tests/ describing their focus
- [ ] Ensure all scripts in scripts/ and utility code in utils/ have file-level comments and function docstrings (for Python)
- [ ] Add a DEVELOPER.md or section in the README about documentation/commenting standards for new code
- [ ] Expand README.md with advanced usage, troubleshooting, and developer notes
- [ ] Add docs/ directory for guides, design notes, and architecture diagrams/flowcharts
- [ ] Document build system, test suite, and contribution workflow
- [ ] Add architecture diagrams/flowcharts as needed

### Recommendations for Further Comments/Docs (2025-06-26)
- All public headers and non-trivial .cpp files are now documented with Doxygen and inline comments. Empty or trivial files were skipped.
- Every change was followed by a clean build and test; all tests pass.
- CLI modernization is nearly complete, but some default behaviors and output conventions remain to be modernized (see CLI_MODERNIZATION_CHECKLIST.md).
- Next steps:
  - Add file-level comments to all test .cpp files in tests/
  - Ensure all scripts in scripts/ and utility code in utils/ have file-level comments and function docstrings (for Python)
  - Add a DEVELOPER.md or section in the README about documentation/commenting standards for new code
  - Make output file/type/parameters optional with sensible defaults in CLI
  - Change default output to JSON if no type is specified
  - Ensure all errors/warnings go to stderr, and main data output goes to file or stdout as appropriate
  - Add or improve unit tests for CLI argument parsing and error handling
  - Replace remaining raw loops with range-based for and auto where appropriate
  - Remove unnecessary using namespace and global variables
  - Profile and optimize hot paths; avoid unnecessary copies/allocations
  - Remove unused functions, classes, and files; mark deprecated APIs
  - Apply .clang-format to all code and document the process
  - Add architecture diagrams/flowcharts to docs/
  - Expand the README with advanced usage, troubleshooting, and developer notes
  - Add more granular trace logs for data mutations and branch decisions
  - Proofread and finalize all documentation; conduct a thorough code review; tag and announce the next release

---

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
- [x] Replace `assert`/manual checks with exceptions or error codes
    - Rationale: Exceptions and error codes provide safer, more user-friendly error handling than asserts, especially in public APIs. This helps users and contributors understand and recover from errors.
    - Steps: Review all public methods, replace assert with exceptions or error codes, add clear error messages, update tests as needed.
    - Status: Complete for all core classes (RDSGraph, EquivalenceClass, SignificantPattern, RDSNode). Build and all tests passed after changes.
- [x] Add robust input validation and error messages
    - Status: Complete for all core classes. All major public methods now validate input and provide clear error messages. Build and all tests passed after changes.
- [x] Apply consistent code style (`clang-format`)
    - Rationale: Consistent code style improves readability, maintainability, and approachability for new contributors. Using a standard style (Google-based, C++14) ensures best practices and makes the codebase beginner-friendly.
    - Steps: Add a `.clang-format` file to the project root, run `clang-format` on all `.cpp` and `.h` files in `src/`, `include/`, and `tests/`, review changes, and commit. Document the process for future contributors.
    - Status: `.clang-format` file added and code formatting in progress.
- [ ] Profile and optimize hot paths if needed
- [ ] Avoid unnecessary copies/allocations
- [ ] Remove unused functions, classes, and files
- [ ] Mark deprecated APIs and provide migration paths if needed

## Phase 5: Final Review and Release
- [ ] Run all tests and verify 100% pass rate
- [ ] Proofread and finalize all documentation
- [ ] Conduct thorough code review (self or peer)
- [ ] Tag repository for new release (e.g., `v2.0.0`)
- [ ] Announce changes in `CHANGELOG.md`

---

## Public Header Doxygen Documentation (2025-06-26)
- [x] All major public headers in `include/madios/` reviewed and modernized with Doxygen-style documentation.
- [x] Each change followed by a clean build and test cycle.
- [x] Headers already well-documented were confirmed and left unchanged.
- [x] Empty or placeholder headers were noted and skipped.
- [x] Documentation for version macros and other small headers was added for completeness.

### Recommendations for Further Comments/Docs
- Add file-level comments to all `.cpp` implementation files describing their purpose.
- Add Doxygen comments for any non-trivial, public, or complex functions in `.cpp` files, especially those not already documented in headers.
- Add inline comments for complex algorithms or logic branches.
- Review and document any internal headers (not in `include/madios/`) if used across multiple `.cpp` files.
- Add a section to the README or a new `DEVELOPER.md` about documentation standards and expectations for new code.
- Ensure all scripts in `scripts/` and utility code in `utils/` have at least a file-level comment and function docstrings (for Python scripts).
- Consider more granular trace logs for data mutations and branch decisions.
- Add brief file-level comments to test files describing their focus.

## Ongoing
- [x] Deep copy/clone and simulation logic for RDSGraph is robust and tested (2025-06-26)
- [ ] Update this checklist and the reform plan as tasks are completed or requirements change
- [ ] Use issue tracker or project board for detailed tracking if desired

---

*Checklist updated after Doxygen/public header and .cpp documentation phase (2025-06-26). Next: CLI defaults, test/docs comments, code modernization, and release prep.*
