# SymbioRAP MemoryBench

**An architecture-neutral benchmark for persistent AI/agent memory, durable storage, retrieval, and recovery.**

SymbioRAP MemoryBench is the open benchmark framework used for the M14 publication of the SymbioRAP / SynapseFS research program. It is designed to make performance, correctness, durability, recovery, and retrieval measurements reproducible without assuming a particular memory-engine architecture.

## What this repository is

This repository contains the public benchmark methodology, workload definitions, neutral adapter contract, external comparator adapters, verification tooling, and the M14 reference results.

It intentionally does **not** contain the proprietary SynapseFS or SymbioRAP Memory Engine implementation.

## Benchmark principles

- preregister workloads, seeds, repetition counts, and thresholds before promotion runs;
- retain every run and do not delete inconvenient outliers;
- record hardware, operating system, compiler, library, and storage metadata;
- separate workloads by semantic class instead of turning unlike systems into a single leaderboard;
- preserve negative results and configuration-specific failures;
- publish medians, dispersion, raw-count provenance, and limitations;
- do not infer cross-platform winners from measurements made on different machines.

## Semantic classes

| Class | Purpose | Examples |
| --- | --- | --- |
| `memory-e2e` | End-to-end persistent memory ingest/query behavior | Memory-engine adapter |
| `durable-kv` | Matched durable single-key storage operations | SQLite, RocksDB |
| `retrieval-only` | Vector retrieval only; no durability equivalence implied | FAISS IndexFlatL2 |
| `recovery` | Crash/restart/idempotency behavior | process-kill campaigns |
| `scaling` | Intra-host concurrency behavior | worker scaling / group commit |

## M14 reference evidence

The M14 reference publication records native Windows x64 and Linux ARM64 promotion runs separately. The published claim matrix allows narrowly scoped statements about correctness, recovery, task accuracy, group-commit behavior, intra-host scaling, and successful execution of genuine external comparators. It explicitly blocks claims such as “universally faster than SQLite/RocksDB/FAISS” or cross-platform rankings.

See [`results/m14-reference/`](results/m14-reference/) and [`docs/known-limitations.md`](docs/known-limitations.md).

## Adapter contract

A benchmarked system should expose equivalent lifecycle operations through an adapter, conceptually:

```text
setup()
ingest(record)
query(query)
persist()
reopen()
delete()
recover()
stats()
```

The exact Python protocol used by the open harness lives in [`benchmark/adapters/base.py`](benchmark/adapters/base.py). A non-proprietary example is provided in [`benchmark/adapters/reference.py`](benchmark/adapters/reference.py).

## External comparators

The repository includes C++ adapters for:

- SQLite — durable single-key KV profile, WAL + synchronous FULL where selected;
- RocksDB — durable single-key KV profile with synchronous writes where selected;
- FAISS — retrieval-only `IndexFlatL2`; it must never be interpreted as a durability comparator.

Comparator versions, build flags, and platform details must be reported with every result.

## Reproducing the M14 publication

Start with [`docs/reproducibility.md`](docs/reproducibility.md). Reference results are under [`results/m14-reference/`](results/m14-reference/).

The original M14 evidence was generated from immutable promotion bundles. This public repository intentionally publishes the normalized results and hashes rather than the proprietary system implementation.

## Submitting results

Community results are welcome when they include:

1. the exact MemoryBench release/tag;
2. a complete environment manifest;
3. unmodified workload definitions;
4. all repetitions, including negative results;
5. adapter source or a sufficiently auditable adapter description;
6. semantic-class declaration for every compared system.

Do not submit a single “winner” score that mixes non-equivalent semantic classes.

## License

Code in this repository is licensed under the Apache License 2.0. Reference result data and documentation may be cited with attribution; see [`CITATION.cff`](CITATION.cff).

## Project

Published by General Informatics as part of the SymbioRAP research program.
