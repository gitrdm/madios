# Madios

This project started off as a clone of https://github.com/shaobohou/madios. The goal was to experiment using an LLM to refactor and modernize the codebase, improving readability, maintainability, and performance while preserving the original functionality. This was a bit of a tough challenge, as the LLM tended to add random text that would break the build even if you were only adding comments or Doxygen comments. To mitigate this, I used checklists and a step-by-step migration process to ensure that each change was carefully reviewed and tested before moving on to the next step. The process got done quicker than I could do it, but it was still a lot of work. Compounding the challenge, is that I am not a C++ programmer and so the LLM had to do a lot of the heavy lifting in terms of understanding the code and making meaningful changes. But the regression tests are passing and I think the end result is a more modern, readable, and maintainable codebase that still retains the original functionality of the ADIOS algorithm as intended by the original authors -- at least according to the LLM :).

So, use at your own risk. The code has not been extensively tested and may contain bugs or undiscovered regressions. The original authors of the ADIOS algorithm and I are not responsible for any issues that may arise from using this code.

Madios is a modern C++ implementation of the ADIOS grammar induction algorithm.

## Directory Structure

- `include/madios/` — Public headers for library and CLI
- `src/` — Implementation files (C++ sources)
- `tests/` — Test sources, test data, and scripts
- `test/` — Test data for legacy/test compatibility
- `ext/` — External dependencies
- `build/` — Build artifacts (ignored by git)
- `utils/` — Utility implementations
- `maths/` — Math-specific code (now under `include/madios/maths/`)

## Building

```sh
mkdir -p build
cd build
cmake ..
make -j$(nproc)
ctest --output-on-failure
```

## Contributing
See `CONTRIBUTING.md` for guidelines and rationale.

**Purpose:**
This project implements the ADIOS (Automatic DIstillation Of Structure) algorithm for unsupervised grammar induction from text corpora. It reads a set of sentences and induces grammatical patterns, which can be used for natural language processing and parser development.

## Project Structure

- `src/` – All implementation files (C++ sources)
- `include/madios/` – All public headers
- `include/madios/utils/` – Utility headers
- `include/madios/maths/` – Math headers
- `include/madios/maths/tnt/` – Math library headers
- `tests/` – All test code and test data

## Building

### With CMake (Recommended)

```sh
mkdir -p build
cd build
cmake ..
make
```

The resulting executable will be `build/madios`.

## Command-Line Usage

```
./build/madios <filename> <eta> <alpha> <context_size> <coverage> [-o <outputfile>] [--format <format>] [number_of_new_sequences]
```

- `<filename>`: Path to the input corpus file (see below for format).
- `<eta>`: Threshold for detecting divergence in the RDS graph (e.g., 0.9).
- `<alpha>`: Significance test threshold (e.g., 0.01 or 0.1).
- `<context_size>`: Size of the context window for equivalence class search (e.g., 5).
- `<coverage>`: Threshold for bootstrapping equivalence classes (e.g., 0.65).
- `-o <outputfile>`: (Optional) Write all output to the specified file instead of stdout.
- `--format <format>`: Output format: `json`, `pcfg`, or `text` (default: `text`).
- `[number_of_new_sequences]`: (Optional) Number of new sequences to generate from the grammar.

## Command-Line Options

| Option/Arg                  | Description                                                                 | Default         |
|-----------------------------|-----------------------------------------------------------------------------|-----------------|
| `<filename>`                | Input corpus file (required)                                                | —               |
| `<eta>`                     | Divergence threshold (e.g., 0.9, required)                                  | 0.9             |
| `<alpha>`                   | Significance threshold (e.g., 0.01, required)                               | 0.01            |
| `<context_size>`            | Context window size (e.g., 5, required)                                     | 5               |
| `<coverage>`                | Coverage threshold (e.g., 0.65, required)                                   | 0.65            |
| `[number_of_new_sequences]` | Number of new sequences to generate from the grammar                        | 0               |
| `-o`, `--output`            | Output file (writes all output to file instead of stdout)                   | stdout          |
| `--format <format>`         | Output format: json, pcfg, or text (default: text)                          | text            |
| `--verbose`                 | Enable verbose progress/info output                                         | off             |
| `--quiet`                   | Suppress all non-error output (overrides --verbose)                         | off             |

### Output Behavior
- Output is printed to stdout or the file specified by `-o`/`--output`, regardless of format.
- Progress and info messages are printed only if `--verbose` is set and `--quiet` is not set.
- Errors are always printed to stderr.

### Example Usage

```sh
./build/madios test/test_madios.txt 0.9 0.01 5 0.65 --format json
./build/madios test/test_madios.txt 0.9 0.01 5 0.65 --format pcfg -o grammar.txt
./build/madios test/test_madios.txt 0.9 0.01 5 0.65 --verbose
```

## Input Corpus Format

Each line is a sentence. ADIOS-style input uses `*` and `#` as start/end markers, but plain space-separated text is also accepted. Example:

```
* Cindy believes that Joe believes that to please is easy #
* Cindy believes that Cindy believes that to read is tough #
* Pam thinks that Jim believes that to please is tough #
...
```

## Example PCFG Output

When run with `--format pcfg`, the output will look like:

```
E27 -> believes [0.730769]
E27 -> thinks [0.269231]
P28 -> E27 that [1]
E29 -> read [0.5]
E29 -> please [0.5]
P30 -> P28 to E29 is [1]
E31 -> Cindy [0.4]
E31 -> Jim [0.1]
E31 -> Beth [0.1]
E31 -> Joe [0.4]
P32 -> P28 E31 P30 [1]
E33 -> cat [0.3]
E33 -> cow [0.1]
E33 -> horse [0.4]
E33 -> dog [0.2]
P34 -> the E33 [1]
P35 -> to E29 [1]
E36 -> easy [0.666667]
E36 -> eager [0.333333]
P37 -> is E36 P35 [1]
S -> Beth P28 George P30 easy [0.05]
S -> Beth P32 tough [0.15]
S -> Cindy P28 George P30 easy [0.05]
S -> Cindy P32 easy [0.15]
S -> Cindy P32 tough [0.05]
S -> Pam P28 Pam P30 easy [0.05]
S -> Pam P32 easy [0.1]
S -> Pam P32 tough [0.05]
S -> that Cindy P37 annoys Cindy [0.05]
S -> that Cindy P37 bothers P34 [0.05]
S -> that P34 P37 annoys Jim [0.05]
S -> that P34 P37 annoys P34 [0.05]
S -> that P34 P37 disturbs P34 [0.05]
S -> that P34 P37 worries P34 [0.05]
S -> that P34 is tough P35 disturbs P34 [0.05]
```

Probabilities are normalized for each LHS (left-hand side).

## Output Details

- **Default output:** Human-readable, includes corpus, lexicon, search paths, and grammar.
- **-o <outputfile>:** All output is written to the specified file instead of stdout.
- **--format json:** Machine-readable JSON with all results.
- **--format pcfg:** Only the PCFG rules, suitable for downstream parsing tools.

## Running Tests

This project uses [Catch2](https://github.com/catchorg/Catch2) for unit testing. To set up and run tests:

1. Download the Catch2 single-header file (if not already present):

```sh
curl -L -o tests/catch.hpp https://github.com/catchorg/Catch2/releases/download/v2.13.10/catch.hpp
```

2. Build and run tests:

```sh
./build_and_install.sh
./run_all_tests.sh
```

Or, to run the main C++ test binary directly:

```sh
./build/tests_basic
```

Or, to run all tests with CTest:

```sh
cd build
ctest --output-on-failure
```

### CLI Output Test

A shell script test is provided to check the CLI `--pcfg` output:

```sh
cd tests
./test_cli_pcfg_output.sh
```

This checks that the CLI produces valid PCFG output and that probabilities are normalized.

## Further Reading

- [ADIOS Algorithm Paper](https://www.cs.bgu.ac.il/~elhadad/nlp09/ADIOS.pdf)
- [C++ Language Reference](https://en.cppreference.com/)

## License

See `License.txt` for details. This project is provided under a permissive license (original and MIT).
