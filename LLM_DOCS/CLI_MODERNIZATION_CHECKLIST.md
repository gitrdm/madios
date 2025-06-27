# C++ CLI Application Modernization Checklist

## 1. Command-Line Interface (CLI) Cleanup
- [x] Use a modern CLI parsing library (e.g., CLI11, cxxopts, argparse) for consistent option handling.
- [x] Define all options and arguments in a single place (e.g., main.cpp or a dedicated CLI module).
- [x] Use clear, descriptive names for all options (e.g., `--input`, `--output`, `--verbose`, `--help`).
- [x] Provide both short (`-i`) and long (`--input`) forms for options where appropriate.
- [x] Ensure all options have clear help descriptions.
- [x] Validate required arguments and provide helpful error messages for missing/invalid input.
- [x] Support a `--help`/`-h` flag that prints usage and exits cleanly.
- [ ] Output file name is optional; default is the root of the input file name plus the output file extension.
- [ ] Output type is optional; default output is JSON.
- [ ] Parameters are optional and default to reasonable values.

## 2. Output and Logging Improvements
- [ ] Change default behavior: normal mode should read from input file and write to output file.
- [ ] Use stdout only for progress, errors, and informational messages (not for main data output).
- [ ] Add a `--verbose` or `--quiet` flag to control the level of informational output.
- [ ] Print errors and warnings to stderr, not stdout.
- [ ] Use a lightweight logging utility or a simple logging function for info/progress/error messages.
- [ ] Clearly separate CLI/IO logic from core processing logic for easier testing and reuse.

## 3. General Codebase Modernization
- [ ] Remove or refactor legacy/unused command-line options.
- [ ] Document all CLI options and expected input/output formats in the README.
- [ ] Add unit tests for CLI argument parsing and error handling.
- [ ] Ensure the application exits with appropriate status codes (0 for success, nonzero for errors).
