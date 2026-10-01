# SymbioRAP / SynapseFS M14 Open Benchmark Publication

Publication scope: frozen M13 RC4.3 promotion evidence. Results are platform- and workload-specific.

## Native correctness and recovery

| Platform | Correctness suite | H7 abrupt-termination campaign |
|---|---:|---:|
| Windows x64 | 58/58 PASS | 100/100 PASS |
| Linux ARM64 | 62/62 PASS | 100/100 PASS |

## H6 — full Memory Engine path

Ten independent repetitions per platform; no samples discarded.

| Platform | Median ingest ops/s | Ingest CV | Median query ops/s | Query CV | Task accuracy |
|---|---:|---:|---:|---:|---:|
| Windows x64 | 154.78 | 37.70% | 1758.49 | 8.73% | 1.0 |
| Linux ARM64 | 137.71 | 4.67% | 551.55 | 15.91% | 1.0 |

H6 values are not directly comparable with SQLite/RocksDB durable-KV results because the measured semantics differ.

## H8 — scaling and durable group commit

| Platform | 1 worker median ops/s | 32 workers median ops/s | 32-worker CV | WAL records/ingest | WAL syncs/ingest | Avg group batch |
|---|---:|---:|---:|---:|---:|---:|
| Windows x64 | 264.00 | 749.48 | 1.45% | 4.000 | 0.763 | 5.251 |
| Linux ARM64 | 129.13 | 1143.26 | 15.77% | 4.000 | 0.314 | 12.772 |

This is within-host scaling evidence, not a Windows-versus-Linux ranking.

## External adapters

| Platform | Adapter | Version | Configuration | Correctness | Median throughput | CV | Semantic class |
|---|---|---|---|---|---:|---:|---|
| Windows x64 | SQLite | 3.53.4 | matched-full | PASS | 1132.88 ops/s | 1.32% | durable-kv |
| Windows x64 | RocksDB | 11.8.1 | matched-sync | PASS | 1175.84 ops/s | 1.46% | durable-kv |
| Windows x64 | FAISS | 1.14.3 | IndexFlatL2-default | NEGATIVE RESULT | 245.61 q/s | 6.36% | retrieval-only |
| Linux ARM64 | SQLite | 3.45.1 | matched-full | PASS | 1304.90 ops/s | 5.33% | durable-kv |
| Linux ARM64 | RocksDB | 8.9.1 | matched-sync | PASS | 866.83 ops/s | 10.80% | durable-kv |
| Linux ARM64 | FAISS | 1.14.1 | IndexFlatL2-default | PASS | 180.95 q/s | 2.72% | retrieval-only |

SQLite/RocksDB are matched durable single-key KV adapters. FAISS is retrieval-only. Their throughput values must not be used as direct full-Memory-Engine rankings.

## Publication rule

Only claims marked public in `claim-matrix.json` are authorized. Negative results and platform-specific limitations are retained.
