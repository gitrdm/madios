# LLM Migration Context: Lessons Learned from Previous Migration Attempt

## Key Lessons and Warnings

- **Context Size and EC Creation:**
  - The original code and test use `contextSize=2` (see `tests/test_pcfg_output.cpp`).
  - With `contextSize=2`, the slot loop `for(unsigned int j = 1; j < params.contextSize-1; j++)` does not execute, so no ECs are created. The test still passes, indicating that ECs are not required for this minimal case.
  - **Do not change the slot loop bounds to include 0 or `contextSize-1`** without a full audit of EC logic and edge cases. Doing so caused out-of-bounds errors and segfaults in the previous migration.
  - For robust and safe EC creation (if ECs are required), use `contextSize=3` or higher in tests and production runs.

- **Test Consistency:**
  - Always verify that the test parameters match the intended algorithmic behavior. If ECs are expected, ensure `contextSize` is set appropriately.
  - Document any changes to test parameters and the rationale for those changes.

- **Migration Pitfalls:**
  - Avoid making changes to core algorithmic loop bounds or invariants unless you fully understand the implications for memory safety and algorithm correctness.
  - If you must change such logic, add assertions and extensive debug output to catch issues early.
  - Always compare migrated code to the original for subtle differences in slot handling, EC creation, and parse tree rewiring.

- **Debugging and Rollback:**
  - If a migration introduces segfaults or test failures, revert to the last known-good state and review all changes to loop bounds, EC handling, and test parameters.
  - Use `git clean -fdx` and `git reset --hard HEAD` to clean up the workspace before restarting a migration.

## Migration Checklist for LLM

1. **Preserve original slot loop bounds for EC creation:**
   - Use `for(unsigned int j = 1; j < params.contextSize-1; j++)` unless you have a strong, well-tested reason to change it.
2. **If ECs are required, set `contextSize=3` or higher in tests.**
3. **Do not attempt to create ECs for `contextSize=2` unless the entire EC logic is audited and updated for edge cases.**
4. **After each migration step, run all tests and check for segfaults or logic errors.**
5. **If a test fails or segfaults, review recent changes to slot loop bounds, EC creation, and parse tree logic first.**
6. **Document all changes and their rationale in the migration audit trail.**

## Reference
- The premigration reference code is in `RDSGraph.cpp.master`.
- The slot loop for EC creation should be: `for(unsigned int j = 1; j < params.contextSize-1; j++)`.
- See `PCFG_OUTPUT_DEBUG_CHECKLIST.md` for a detailed audit trail of the previous migration attempt.
- The original test uses `contextSize=2` and passes without ECs being created.

---

**Summary:**
- The original test uses `contextSize=2` and does not require ECs.
- Do not change slot loop bounds without a full audit.
- If ECs are required, use `contextSize=3` or higher.
- Always compare with the original code and document all changes.
- Clean up the workspace before starting a new migration.
