# M14 Publication Methodology

M14 publishes only measurements collected by the frozen M13 RC4.3 promotion framework. It does not alter M11/M12/M13 workloads, thresholds, durability semantics, comparator semantics, seeds already present in the raw evidence, or raw samples.

## Populations

Windows native x64 and Linux native ARM64 are separate populations. Their absolute throughput is not used to rank operating systems or architectures because hardware, storage, toolchains, and comparator library versions differ.

## Repetitions and statistics

H6 and H8 promotion measurements use ten independent repetitions per platform. External comparators use ten repetitions. No samples are silently removed. Published statistics retain n, median, mean, sample standard deviation, coefficient of variation, min, max, and the M13 normal-approximation 95% confidence-interval half-width.

## Correctness and recovery

Publication requires a passing native correctness suite and a completed H7 abrupt-termination recovery campaign. The frozen evidence contains 58/58 Windows tests, 62/62 Linux tests, and 100/100 H7 recovery runs on each platform.

## Comparator boundary

SQLite and RocksDB are genuine external libraries configured for matched durable single-key operations. They are not semantically equivalent to full H6 Memory Engine ingest. FAISS IndexFlatL2 is retrieval-only and is never treated as a WAL/durability comparator.

## Negative evidence

Negative results are retained. The Windows default FAISS configuration exhibited transient retrieval correctness failures in the frozen promotion evidence; a separately labeled single-thread diagnostic passed. The Linux default FAISS run passed. This is published as a configuration-specific observation, not a general claim about FAISS.

## Claim gate

Only wording explicitly marked public in the claim matrix is authorized by M14. NOT_SUPPORTED claims remain prohibited even if someone can derive a visually appealing ratio from non-equivalent metrics.
