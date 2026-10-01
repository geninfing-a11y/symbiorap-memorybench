# Known limitations

- Results describe two concrete tested hosts, not universal hardware behavior.
- Windows and Linux performance values are separate populations.
- Windows H6 ingest has high variance and is retained without filtering.
- Linux H8 high-worker measurements have higher variance than the Windows high-worker population.
- External library versions differ by platform.
- SQLite/RocksDB durable-KV semantics are narrower than full SymbioRAP Memory Engine ingest.
- FAISS is retrieval-only and cannot support durability comparisons.
- The Windows FAISS negative result is limited to the tested Windows/vcpkg/OpenBLAS/runtime configuration.
- The Linux environment collector did not identify FAISS through its package lookup, but the executed comparator self-reported FAISS 1.14.1; the executed comparator result is authoritative for that run.
- Git commit identity was unavailable in the Linux evidence; source identity is anchored by VERSION and the recorded source-manifest SHA-256.
