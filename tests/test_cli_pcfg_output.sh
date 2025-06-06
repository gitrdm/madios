#!/bin/bash
# Test that the madios CLI --pcfg option outputs valid PCFG rules and normalization
set -e

MADIOS_BIN="../build/madios"
CORPUS="../test/test_madios.txt"
TMP_OUT="cli_pcfg_tmp.txt"

if [ ! -x "$MADIOS_BIN" ]; then
  echo "madios binary not found at $MADIOS_BIN"
  exit 1
fi

# Run madios with --pcfg and capture output
$MADIOS_BIN $CORPUS 0.9 0.01 2 0.5 --pcfg > "$TMP_OUT"

# Check that at least one PCFG rule is present
RULE_COUNT=$(grep -E '^[A-Za-z0-9_]+ -> .+ \[[0-9.eE+-]+\]$' "$TMP_OUT" | wc -l)
if [ "$RULE_COUNT" -eq 0 ]; then
  echo "No PCFG rules found in output!"
  exit 2
fi

# Check normalization for a specific LHS (e.g., E0 if present)
if grep -q '^E0 ->' "$TMP_OUT"; then
  SUM=$(grep '^E0 ->' "$TMP_OUT" | sed -E 's/.*\[([0-9.eE+-]+)\]/\1/' | awk '{s+=$1} END {print s}')
  # Allow for floating point error
  python3 -c "import sys; s=float('$SUM'); sys.exit(0 if abs(s-1.0)<1e-6 else 3)" || { echo "Probabilities for E0 do not sum to 1: $SUM"; exit 3; }
fi

echo "CLI PCFG output test passed."
rm -f "$TMP_OUT"
exit 0
