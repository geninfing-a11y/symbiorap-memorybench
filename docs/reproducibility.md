# Reproducing the M14 publication

Use the immutable M13 evidence ZIPs with the M14 publication generator from the corresponding source release. The public repository contains the normalized reference results, workload definitions, verifier, and semantic rules.

For the published reference snapshot:

```bash
python3 scripts/verify_reference.py
```

The verifier uses only the Python standard library. Re-running performance benchmarks requires the benchmarked system or adapter and the exact environment manifest used for that run.
