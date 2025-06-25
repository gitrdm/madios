# Contributing to Madios

## Project Structure Rationale

Madios uses a modern, modular directory structure:

- `include/madios/` — Public headers for library and CLI
- `src/` — Implementation files (C++ sources)
- `tests/` — Test sources, test data, and scripts
- `test/` — Test data for legacy/test compatibility
- `ext/` — External dependencies
- `build/` — Build artifacts (ignored by git)
- `utils/` — Utility implementations
- `maths/` — Math-specific code (now under `include/madios/maths/`)

This structure supports CMake, IDEs, and packaging. See `README.md` for more details.

## Contribution Guidelines

- Fork and branch from `main` or latest stable.
- Write clear, well-documented code and tests.
- Run `make` and `ctest` before submitting PRs.
- Follow code style (`.clang-format`).
- Update documentation for any user-facing changes.
- Use the issue tracker for bugs and feature requests.

Thank you for contributing!
