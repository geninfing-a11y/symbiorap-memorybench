# SymbioRAP MemoryBench v1.0.0-m14

First public release of the open, architecture-neutral SymbioRAP MemoryBench.

## Included

- frozen public benchmark workload definitions;
- architecture-neutral adapter contract and reference adapter;
- genuine SQLite, RocksDB, and FAISS comparator sources;
- Windows x64 and Linux ARM64 M14 normalized reference results;
- public claim matrix and known-limitations documentation;
- cross-platform CI verification;
- manual SQLite comparator smoke workflow;
- citation metadata and Apache-2.0 licensing.

## Reference evidence

The M14 publication preserves the following platform-specific results:

- Windows x64 correctness: 58/58 PASS;
- Linux ARM64 correctness: 62/62 PASS;
- abrupt-termination recovery: 100/100 PASS on each tested platform;
- H6 task accuracy: 1.0 across all promotion repetitions;
- genuine SQLite and RocksDB durable-KV comparator execution on both platforms;
- FAISS default correctness PASS on the tested Linux configuration;
- FAISS default negative retrieval result retained for the tested Windows configuration, with the separately labeled Windows single-thread diagnostic PASS.

See `results/m14-reference/claim-matrix.json` for the exact publication boundary.

## Interpretation boundary

This release does **not** claim that SymbioRAP/SynapseFS is universally faster than SQLite, RocksDB, or FAISS. The comparator semantic classes differ. Windows and Linux results are separate hardware/software populations and must not be used as an OS or architecture ranking.

## Proprietary code

The proprietary SynapseFS and SymbioRAP Memory Engine implementation is not part of this repository. This release publishes the benchmark framework, methodology, comparator sources, verification tooling, and normalized reference evidence only.

## Reproducibility

Run:

```bash
python scripts/verify_reference.py
python scripts/verify_m14_publication.py
```

The normal CI runs these publication checks on Ubuntu and Windows.

## Citation

Use the included `CITATION.cff` and cite the exact release tag `v1.0.0-m14`.
