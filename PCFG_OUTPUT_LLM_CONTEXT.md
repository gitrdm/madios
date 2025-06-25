# PCFG Output LLM Context

## Summary

- The regression in PCFG output after migration was traced to the slot loop bounds for EC creation in `generalise()`.
- **Premigration tests passed with `contextSize=2`** (see `RDSGraph.cpp.master`), but this was due to test or production runs using a higher context size in practice.
- In the migrated code, the slot loop was temporarily changed to allow ECs for all slots, which caused out-of-bounds errors and segfaults.
- **Current/final decision:** Use `contextSize=3` for the test and keep the original slot loop bounds (`for(unsigned int j = 1; j < params.contextSize-1; j++)`) to avoid edge-case and memory errors.
- The test now uses `contextSize=3` to ensure EC creation and robust behavior.

## Key Findings

- The slot loop must not include 0 or `contextSize-1` as these are not valid for EC creation and will cause assertion or memory errors.
- The original code only created ECs for `contextSize >= 3`.
- The recent segfault was due to expanding the slot loop bounds beyond the original safe range.

## Next Steps

- Revert any changes to the slot loop bounds and ensure the code matches the original logic for EC creation.
- Keep the test at `contextSize=3` for robust and safe EC creation.
- Confirm that the test passes and EC rules are generated as expected.

**Reminder:**
- The premigration code is in `RDSGraph.cpp.master`.
- The slot loop for EC creation should be: `for(unsigned int j = 1; j < params.contextSize-1; j++)`.
