# Madios Refactor & Modernization Checklist

Use this checklist to track progress through the reform plan. Check off each item as you complete it. Update as needed.

---

## Phase 1: Robust Regression Test Suite
- [ ] Audit all existing tests in `tests/` directory
- [ ] Identify and document test coverage gaps
- [ ] Add unit tests for uncovered logic (e.g., `RDSGraph`, `EquivalenceClass`, `SignificantPattern`)
- [ ] Add integration tests for CLI, output formats, and error conditions
- [ ] Ensure tests cover edge cases and invalid input
- [ ] Integrate all tests with CTest
- [ ] Ensure tests run in CI (if available)
- [ ] Add/verify output comparison scripts (`compare_json.py`, `diff_grammar.py`)
- [ ] Create `TESTING.md` with test suite instructions

## Phase 2: Filesystem and Project Structure Refactor
- [ ] Propose and document new directory structure
- [ ] Move source/header files to new locations
- [ ] Update `CMakeLists.txt` and build scripts for new layout
- [ ] Refactor `#include` directives and paths
- [ ] Remove obsolete/duplicate files
- [ ] Ensure all tests and builds pass after each move
- [ ] Update `README.md` and add `CONTRIBUTING.md` with structure rationale

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
