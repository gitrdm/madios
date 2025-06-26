#!/bin/bash
# migrate_source_file.sh
# Robustly migrate a source/header file to a new location, preserving history and safety.
# Usage: ./migrate_source_file.sh <source_file> <destination_file> [--test]
#   --test: Run ./clean_build_and_test.sh after migration and log result.
#
# This script will:
#   - Refuse to move empty or placeholder files (zero size or only comments/whitespace).
#   - Restore the last known good version from git to the destination.
#   - Move the current file to deleteme/ (quarantine) with its relative path.
#   - Log all actions to MIGRATION_STEP_LOG.md.
#   - Optionally run build/tests after migration.

set -euo pipefail

SRC="$1"
DST="$2"
RUN_TESTS="false"
if [[ "${3:-}" == "--test" ]]; then
  RUN_TESTS="true"
fi

LOG="MIGRATION_STEP_LOG.md"
DELETEME="deleteme"

if [[ ! -f "$SRC" ]]; then
  echo "ERROR: Source file '$SRC' does not exist." >&2
  exit 1
fi

# Refuse to move empty or placeholder files
if [[ ! -s "$SRC" ]]; then
  echo "ERROR: Source file '$SRC' is empty. Refusing to migrate." >&2
  exit 2
fi
# Check if file is only whitespace or comments (C++ style)
if grep -v -E '^[[:space:]]*(//|/\*|\*|\*/|$)' "$SRC" | grep -q '.'; then
  : # Has real code
else
  echo "ERROR: Source file '$SRC' contains only comments/whitespace. Refusing to migrate." >&2
  exit 3
fi

# Make sure destination directory exists
DST_DIR=$(dirname "$DST")
mkdir -p "$DST_DIR"

# Restore last known good version from git to destination
if git ls-files --error-unmatch "$SRC" >/dev/null 2>&1; then
  git show HEAD:"$SRC" > "$DST"
  echo "Restored last known good version of $SRC to $DST" | tee -a "$LOG"
else
  echo "WARNING: $SRC not tracked in git. Copying current file to $DST." | tee -a "$LOG"
  cp "$SRC" "$DST"
fi

# Move the current file to deleteme/ quarantine
REL_SRC=$(realpath --relative-to=. "$SRC")
DELETEME_PATH="$DELETEME/$REL_SRC"
mkdir -p "$(dirname "$DELETEME_PATH")"
mv "$SRC" "$DELETEME_PATH"
echo "Moved $SRC to quarantine at $DELETEME_PATH" | tee -a "$LOG"

# Optionally run build/tests
if [[ "$RUN_TESTS" == "true" ]]; then
  echo "Running ./clean_build_and_test.sh after migration..." | tee -a "$LOG"
  if ./clean_build_and_test.sh; then
    echo "Build and tests PASSED after migrating $SRC to $DST" | tee -a "$LOG"
  else
    echo "Build and tests FAILED after migrating $SRC to $DST" | tee -a "$LOG"
    exit 4
  fi
fi

echo "Migration of $SRC to $DST complete." | tee -a "$LOG"
