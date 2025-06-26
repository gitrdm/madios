#!/bin/bash
# fix_variable_names.sh
# Script to automatically fix common variable name typos in the madios codebase.
# Usage: ./fix_variable_names.sh

set -e

# List of variable name corrections (add more as needed)
declare -A VAR_FIXES=(
    [all_generalSlots]=all_general_slots
    [somePatterns]=some_patterns
    [best_patternIndex]=best_pattern_index
    [general2boost]=general2boost
    [all_generalSlots]=all_general_slots
    [somePatterns]=some_patterns
    [best_patternIndex]=best_pattern_index
)

# File patterns to search (edit as needed)
FILE_PATTERNS=(src/*.cpp src/*.h include/**/*.h include/**/*.hpp)

for pattern in "${FILE_PATTERNS[@]}"; do
    for file in $pattern; do
        [ -f "$file" ] || continue
        for wrong in "${!VAR_FIXES[@]}"; do
            right=${VAR_FIXES[$wrong]}
            sed -i "s/\b$wrong\b/$right/g" "$file"
        done
    done
done

echo "Variable name fixes applied."
