#!/bin/bash
# scripts/gen_baseline_log.sh
# Generate and save a known good baseline log for madios regression testing.
# Usage: ./scripts/gen_baseline_log.sh <case> [madios_cli_args]
# Example: ./scripts/gen_baseline_log.sh pcfg --pcfg ...

set -e

if [ $# -lt 1 ]; then
    echo "Usage: $0 <case> [madios_cli_args]" >&2
    echo "  <case> is a label for the run, e.g., 'pcfg', 'json', etc." >&2
    exit 1
fi

CASE="$1"
shift

LOGDIR="$(dirname "$0")/../logs"
LOGFILE="$LOGDIR/baseline_run_${CASE}.log"
BINARY="$(dirname "$0")/../build/madios"

mkdir -p "$LOGDIR"

if [ ! -x "$BINARY" ]; then
    echo "Error: madios binary not found at $BINARY. Build the project first." >&2
    exit 1
fi

# Run the CLI with arguments, capture all output (stdout+stderr) to the log
"$BINARY" "$@" 2>&1 | tee "$LOGFILE"

echo "Baseline log saved to $LOGFILE"
