# Workload semantics

MemoryBench separates results into semantic classes. This is a hard publication rule, not presentation advice.

## memory-e2e
Measures an end-to-end persistent memory pipeline. Results can include indexing, evidence mutation, persistence, retrieval, and product-specific bookkeeping. A memory-e2e throughput number is not directly equivalent to a single durable key-value write.

## durable-kv
Measures matched durable storage operations under an explicitly stated durability profile. SQLite and RocksDB reference adapters belong here.

## retrieval-only
Measures retrieval behavior only. The M14 FAISS adapter uses IndexFlatL2. It does not provide transaction/WAL semantics comparable to durable storage or a full memory engine.

## recovery
Measures restart, crash consistency, replay, and idempotency behavior. Passing recovery campaigns is a correctness property and must not be converted into a throughput score.

## scaling
Measures concurrency behavior within one host and one environment. Scaling data should be interpreted intra-host; it is not a cross-platform hardware ranking.
