# Madios Design Notes and Algorithm Overview

## Overview
Madios implements the ADIOS (Automatic DIstillation Of Structure) algorithm for unsupervised grammar induction. This document provides a high-level overview of the algorithm, design rationale, and architecture.

## ADIOS Algorithm Summary
- Reads a corpus of sentences and builds a graph representation.
- Iteratively discovers significant patterns and generalizes them into equivalence classes.
- Induces a probabilistic context-free grammar (PCFG) from the learned structure.

## Architecture Diagram

```
Corpus Input
     |
     v
[Graph Construction] --> [Pattern Discovery] --> [Generalization] --> [PCFG Output]
```

- **Graph Construction:** Sentences are parsed into nodes and edges.
- **Pattern Discovery:** Statistically significant patterns are found using divergence and significance thresholds.
- **Generalization:** Patterns are abstracted into equivalence classes, updating the graph.
- **PCFG Output:** The final grammar is output in human-readable or machine-readable form.

## Key Modules
- `RDSGraph`: Main graph structure and pattern discovery logic.
- `RDSNode`, `EquivalenceClass`, `SignificantPattern`: Core data structures.
- `main.cpp`: CLI entry point, handles input/output and program flow.

## Further Reading
- See the [ADIOS Algorithm Paper](https://www.cs.bgu.ac.il/~elhadad/nlp09/ADIOS.pdf)
- See Doxygen-generated HTML docs for API reference.
