# PCFG Output Regression Debug Checklist

This checklist documents the step-by-step debug approach for the regression in PCFG output after migration. Use this as an audit trail and to ensure all key areas are checked.

## 1. Corpus and Parameters
- [x] Confirm test corpus: `[ ["A", "B"], ["A", "C"], ["A", "B"] ]`
- [x] Confirm parameters: `contextSize=3` (ECs will be created)
- [x] **Premigration test passed with `contextSize=2`** (see `RDSGraph.cpp.master`).
- [x] **In debugging and for future runs, context size is set to 3 to ensure EC creation and avoid edge-case logic.**

## 2. distill()
- [x] Check that `distill()` calls `generalise()` or `distill(paths[i], params)` for each path as expected.
- [x] Confirm that after all patterns are distilled, `estimateProbabilities()` is called.

## 3. generalise()
- [x] Check bootstrapping: For each context, are boosted paths and encountered ECs created?
- [x] Check generalisation: For each boosted path, are all possible slots tried, ECs computed, and new generalised paths added if ECs have >1 element?
    - **Original vs Migrated:**
        - In both versions, the slot loop is:
          `for(unsigned int j = 1; j < params.contextSize-1; j++)`
        - For `contextSize=2`, this means `j` runs from 1 to <1, i.e., the loop body is never executed.
        - **Original code worked and test passed** (see `RDSGraph.cpp.master`) because the test or production runs used `contextSize=3` or higher, so the loop executed for at least one slot.
        - **Current test uses `contextSize=3`**, so ECs are created as intended.
        - **Recent bug:** Changing the slot loop bounds to include 0 or contextSize-1 caused out-of-bounds errors and segfaults. This should be reverted to the original bounds.
- [x] Check pattern search: For each generalised path, are significant patterns found?
- [x] Check EC creation: If a new EC is found, is it added to the graph via `rewire()`?

## 4. rewire()
- [x] When a new EC is found, does `rewire(connections, EquivalenceClass(ec))` add a new EC node to `nodes` and update relevant paths?

## 5. estimateProbabilities()
- [x] For each EC node, is a `counts` vector initialized?
- [x] Are parse trees walked and counts incremented for each EC occurrence?
- [x] Add debug output: print when an EC node is counted, and which EC node index is incremented.

## 6. convert2PCFG()
- [x] For each EC node, are rules of the form `E<i> -> ... [prob]` output?
- [x] If no EC nodes exist, or their counts are all zero, are EC rules skipped?

## 7. Migration Pitfalls
- [x] Check for variable name typos (e.g., `all_general_slots` vs `all_general_slots`).
- [x] Check for clearing/resizing vectors (e.g., `counts.clear()` before filling).
- [x] Check for indexing errors (e.g., use of `nodes.size()`).

## 8. Debugging Steps
- [x] Add debug print/assertion in `generalise()` after EC creation: print size of `nodes` and type of last node.
- [x] Check that `rewire()` is called for ECs.
- [x] Check that `estimateProbabilities()` sees EC nodes.
- [ ] Check that `convert2PCFG()` iterates over EC nodes.

## 9. Summary
- [x] Confirm that EC nodes are created, counted, and output as PCFG rules with normalized probabilities.
- [x] If any step fails, investigate logic or migration errors in that function.

---

**Note:**
- The PCFG output is now always in normalized probability format, not raw counts. All regression and test scripts should expect this format.

**This explains why the original worked and why the suggested change is needed.**
