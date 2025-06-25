# Madios C++ Codebase Modernization and Refactor Plan

## Overview
This document outlines a comprehensive plan to modernize, refactor, and improve the `madios` C++ codebase. The plan is structured in phases to ensure robust regression protection, maintainability, and clarity throughout the process. Each phase includes concrete steps and deliverables, and this document should be referenced throughout the project to maintain direction and context.

---

## Phase 1: Robust Regression Test Suite

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

---

## Phase 2: Filesystem and Project Structure Refactor

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
     CMakeLists.txt
     README.md
     ...
     ```
2. **Move Files Incrementally**
   - Move source and header files to appropriate directories.
   - Update `CMakeLists.txt` and build scripts to match new layout.
   - Ensure all tests and builds pass after each move.
3. **Update Includes and Paths**
   - Refactor `#include` directives and relative paths.
   - Remove obsolete or duplicate files.
4. **Document Structure**
   - Update `README.md` and add a `CONTRIBUTING.md` with structure rationale and guidelines.

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
