# Madios C++ Codebase Modernization and Refactor Plan

## Overview
This document outlines a comprehensive plan to modernize, refactor, and improve the `madios` C++ codebase. The plan is structured in phases to ensure robust regression protection, maintainability, and clarity throughout the process. Each phase includes concrete steps and deliverables, and this document should be referenced throughout the project to maintain direction and context.

---

## Phase 1: Regression Test Coverage (UPDATED)

### Goals
- Ensure all current functionality is covered by automated tests.
- Enable safe refactoring by catching regressions early.
- Provide a foundation for future feature development and bug fixes.

### Steps
1. **Audit Existing Tests**
   - Catalog all current tests in the `tests/` directory.
   - Identify coverage gaps, especially for edge cases and error handling.
2. **Expand Test Coverage**
   - Write additional unit tests for uncovered logic, especially in core classes like `RDSGraph`, `EquivalenceClass`, and `SignificantPattern`.
   - Add integration tests for CLI behavior, output formats (PCFG, JSON), and error conditions.
   - Ensure tests cover both expected and unexpected input.
3. **Automate Regression Testing**
   - Integrate all tests with CTest for consistent reporting.
   - Ensure all tests are run in CI (if available).
   - Add scripts to compare output files (e.g., `compare_json.py`, `diff_grammar.py`) for robust output validation.
4. **Document Test Suite**
   - Create a `TESTING.md` file describing how to run, extend, and interpret the test suite.

### Additional Test Coverage Gaps (to address):
- Error handling and invalid input (malformed corpus, bad CLI args)
- Exception safety and memory errors
- Full coverage of all public methods (especially new/utility code)
- (Optional) Performance regressions

### Next Steps:
- Add new unit tests for error handling and invalid input
- Add tests for exception safety and memory errors
- Add tests for all public methods in new/utility code
- (Optional) Add performance regression tests

### Phase 1 Status (2025-06-25)
- All regression and output comparison tests pass with the new PCFG probability output format.
- Baseline is set and documented; all future work will use this as the reference for regression.
- Logging, baseline, and traceability infrastructure are complete; all staged code and logs are committed and pushed (2025-06-25).
- Fine-grained trace logging will continue during the refactor phase.
- Documentation, scripts, and test suite are aligned with the new output format.

---

## Logging/Tracing Infrastructure (Pre-Phase 2)

### Rationale
Adding a minimal logging/tracing system before major refactoring provides runtime visibility, helps catch regressions or wiring errors during file moves, and makes debugging easier. It also ensures that logging is consistently integrated and only needs to be updated once as the codebase structure changes.

### Steps
1. **Design and Implement Logging Utility**
   - Create a simple, extensible logger (header + implementation) for use throughout the codebase.
2. **Integrate Logging**
   - Add logging to key modules, entry points, and error paths.
3. **Document Logging Usage**
   - Add usage instructions to README and code comments.
4. **Leverage Logging During Refactor**
   - Use logs to verify correct behavior after each major move or change.
5. **Capture Known Good Baseline Logs**
   - Run CLI and test workflows with logging enabled; save logs as baseline artifacts (e.g., logs/baseline_run.log).
   - Document the process for comparing future logs to this baseline to catch regressions or unexpected changes.
6. **Add Logging at Major Logic Intersections**
   - Instrument function entry/exit, key branches, and data mutations with trace/info logs for deeper data flow visibility.
   - Capture and review enhanced logs for new baseline and future comparison.
7. **Concrete Logging Targets**
   - Add trace/info logging at:
     - All major public method entry/exit in core classes (RDSGraph, EquivalenceClass, SignificantPattern, ParseTree, etc.)
     - Key data mutations (nodes/edges/patterns add/remove/rewire)
     - Important branches and error/exception handling
     - Input/output operations (file open/read/write, CLI start/end)
     - Performance-critical sections (optional timing)
     - Integration points with utility modules

---

## Enhanced Logging for Traceability (Pre-Refactor)

### Goals
- Maximize reproducibility and traceability for all regression baselines.
- Ensure all critical run context and summary information is captured in logs before major refactor.

### Steps
1. **Log Version and Build Info**
   - Log madios version, git commit hash, and build date at CLI start.
2. **Log CLI Arguments and File Names**
   - Log the full CLI command/arguments used for the run.
   - Log input and output file names/paths.
3. **Log Random Seed**
   - Log the deterministic random seed used for reproducibility.
4. **Log Summary Statistics**
   - At the end of each run, log summary stats (patterns found, rewiring ops, final graph size, etc.).
5. **Log Error/Warning Events**
   - Log all error/warning events explicitly.
6. **(During Refactor) Add Fine-Grained Trace Logs**
   - Add more granular trace logs for data mutations and branch decisions in core algorithms.

### Status (2025-06-25)
- All critical run context and summary information is now logged for baseline runs.
- Logging, baseline, and traceability infrastructure are complete; all staged code and logs are committed and pushed (2025-06-25).
- Fine-grained trace logging will continue during the refactor phase.
- Baseline log and output comparison infrastructure is in place and documented.

---

## Phase 2: Filesystem Restructuring (UPDATED)

### Goals
- Adopt a modern, clear, and maintainable project structure.
- Separate core logic, CLI, tests, and utilities.
- Prepare for future modularization and packaging.

### Steps
1. **Propose New Structure**
   - Example:
     ```
     /src         # Core library code
     /include     # Public headers
     /cli         # CLI entry point(s)
     /tests       # Unit and integration tests
     /utils       # Utility scripts (e.g., compare_json.py)
     /docs        # Documentation
     ```
2. **Move Files and Update Includes**
   - Move all public headers to `include/madios/` and update all `#include` directives accordingly.
   - Move test data to `tests/`.
   - Update `CMakeLists.txt` and build scripts for new layout.
   - **[DONE] Fix TNT/Array2D header and namespace usage in all relevant headers and sources.**
   - **[DONE] Synchronize TNT/Array2D and related TNT headers from `src/maths/tnt/` to `include/madios/maths/tnt/` so public headers are complete.**
   - **[x] Synchronize all TNT headers from `src/maths/tnt/` to `include/madios/maths/tnt/` (public headers complete)**
3. **Validate Build and Tests**
   - Ensure all header dependencies are correct and the build passes.
   - Validate that all tests pass after restructuring and header fixes.
   - **[x] Validate build and tests after restructuring**
   - **[x] Update all code to use fully qualified TNT types (e.g., `TNT::Array2D`, `TNT::Array1D`) globally**
   - **[x] Validate build and tests after namespace fix**

---

## Directory Structure Rationale

The new directory structure separates public headers, source files, tests, and external dependencies for clarity and maintainability:

- `include/madios/` — All public headers for library and CLI
- `src/` — All implementation files (C++ sources)
- `tests/` — All test sources, test data, and test scripts
- `test/` — Symlinked or duplicated test data for legacy/test compatibility
- `ext/` — External dependencies (e.g., CLI11, catch.hpp)
- `build/` — Build artifacts (ignored by git)
- `utils/` — Utility implementations (may be merged into src/ in future)
- `maths/` — Math-specific code, now under `include/madios/maths/`

This structure supports modern CMake, IDEs, and packaging. See README for more details.

---

## Phase 3: Documentation Improvement

### Goals
- Make the codebase accessible to new contributors and users.
- Provide clear API, usage, and design documentation.

### Steps
1. **Code Documentation**
   - Add or improve Doxygen-style comments for all public classes, methods, and data structures.
   - Ensure all parameters, return values, and exceptions are documented.
2. **User Documentation**
   - Expand `README.md` with usage examples, installation instructions, and CLI options.
   - Add a `docs/` directory for detailed guides, design notes, and algorithm explanations.
3. **Developer Documentation**
   - Document build system, test suite, and contribution workflow.
   - Add architecture diagrams and flowcharts where helpful.

---

## Phase 4: Code Logic Refactor and Modernization

### Goals
- Improve code readability, maintainability, and performance.
- Adopt modern C++ best practices (C++17/20 where possible).
- Reduce technical debt and eliminate code smells.

### Steps
1. **Refactor in Small, Tested Steps**
   - Use the regression test suite to validate each change.
   - Refactor one module/class at a time (e.g., `RDSGraph`, `EquivalenceClass`).
2. **Modernize C++ Usage**
   - Replace raw pointers with smart pointers where appropriate.
   - Use `auto`, range-based for loops, and STL algorithms.
   - Remove unnecessary `using namespace` and global variables.
   - Prefer `const` correctness and immutability.
3. **Improve Error Handling**
   - Replace `assert` and manual error checks with exceptions or error codes as appropriate.
   - Add robust input validation and error messages.
4. **Optimize Performance**
   - Profile and optimize hot paths if needed.
   - Avoid unnecessary copies and allocations.
5. **Code Style Consistency**
   - Apply a consistent code style (consider using `clang-format`).
   - Add a `.clang-format` file to the repo.
6. **Deprecate/Remove Legacy Code**
   - Remove unused functions, classes, and files.
   - Mark deprecated APIs and provide migration paths if needed.

---

## Phase 5: Final Review and Release

### Goals
- Ensure the codebase is stable, well-documented, and ready for future development.

### Steps
1. **Full Test Pass**
   - Run all tests and verify 100% pass rate.
2. **Documentation Review**
   - Proofread and finalize all documentation.
3. **Code Review**
   - Conduct a thorough code review (self or peer).
4. **Tag and Release**
   - Tag the repository for a new release (e.g., `v2.0.0`).
   - Announce changes in `CHANGELOG.md`.

---

## Ongoing: Reference and Progress Tracking
- Update this plan as tasks are completed or requirements change.
- Use checklists or issue trackers to monitor progress.
- Reference this document whenever context is lost or clarification is needed.

---

## Appendix: Immediate Next Steps
- [ ] Audit and expand regression tests (Phase 1)
- [ ] Propose and discuss new filesystem structure (Phase 2)
- [ ] Begin documentation improvements (Phase 3)
- [ ] Plan first refactor target (Phase 4)

---

## Baseline Log and Script Infrastructure (NEW)

### Goals
- Ensure reproducible, traceable regression testing and output comparison.
- Provide a clear, versioned baseline for logs and outputs.
- Automate baseline generation and comparison for future reforms.

### Steps
1. **Create Directories**
   - Ensure `logs/` exists for baseline and regression logs.
   - Ensure `scripts/` exists for automation and comparison scripts.
2. **Generate and Version Baseline Log**
   - Run CLI and test workflows with logging enabled.
   - Save the resulting log as `logs/baseline_run.log` and version it in the repository.
3. **Document Baseline Generation and Comparison**
   - Add instructions to `TESTING.md` for generating baseline logs and outputs.
   - Add instructions for using scripts in `scripts/` to compare logs and outputs for regression detection.
4. **Review and Update Baseline**
   - After major changes, regenerate and review the baseline log and outputs.
   - Update documentation and scripts as needed to reflect new expectations.

### Status (2025-06-25)
- Baseline log and output comparison infrastructure is in place and documented.
- All future regression detection will use these as reference artifacts.

---

*This plan is a living document. Update as you proceed through each phase.*
