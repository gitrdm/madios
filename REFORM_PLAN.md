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

*This plan is a living document. Update as you proceed through each phase.*
