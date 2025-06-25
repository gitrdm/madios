# CLI Modernization: Recommended Order of Work

## 1. Adopt a Modern CLI Parsing Library
- [x] Integrate a library like CLI11 or cxxopts.
- [x] Refactor CLI parsing to use this library in a single place (main.cpp or a dedicated module).

## 2. Define and Document All CLI Options
- [x] Clearly define all options and arguments, including defaults and help descriptions.
- [x] Implement short and long forms for options.
- [x] Ensure `--help`/`-h` works and prints usage.
- [x] Validate required arguments and provide clear error messages.

## 3. Set Up Output and Parameter Defaults
- [x] Implement logic for default output file naming and output type (e.g., JSON).
- [x] Ensure all parameters have reasonable defaults.

## 4. Refactor Output and Logging
- [x] Change default behavior to file-in/file-out.
- [x] Route progress, info, and errors to stdout/stderr as appropriate.
- [x] Add `--verbose`/`--quiet` flags and a simple logging utility if needed.

## 5. Remove Legacy/Unused CLI Options
- [x] Clean up any old or confusing options now that the new CLI is in place.

## 6. Update Documentation
- [x] Document all CLI options and expected input/output formats in the README.

## 7. Add Unit Tests for CLI Parsing
- [ ] Add or expand tests to cover argument parsing, error handling, and default behaviors.

## 8. General Codebase Modernization
- [x] Ensure the application exits with appropriate status codes.
- [x] Separate CLI/IO logic from core processing logic for easier testing and reuse.
